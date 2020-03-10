#include <stdio.h>
#include <GLFW/glfw3.h>

#include "decoder.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, const char **argv)
{
	GLFWwindow *window;

	if (!glfwInit())
	{
		err_log("Couldn't init GLFW");
		return 1;
	}

	window = glfwCreateWindow(1280, 720, "QUIC streamer", NULL, NULL);
	if (window == NULL)
	{
		err_log("Couldn't open window");
		glfwTerminate();
		return 1;
	}

	decoder_t dec;
	if (!quicsy_decoder_open(&dec, argv[1]))
	{
		err_log("Couldn't open video file");
		return 1;
	}
	log("opening the input file (%s) and loading format (container) header", argv[1]);

	glfwMakeContextCurrent(window);

	// Generate texture
	GLuint tex_handle;
	glGenTextures(1, &tex_handle);
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Allocate frame buffer
	const int frame_width = dec.width;
	const int frame_height = dec.height;
	uint8_t *frame_data = new uint8_t[frame_width * frame_height * 4];

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up orphographic projection
		int window_width, window_height;
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, window_width, window_height, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		// Read a new frame and load it into texture
		int64_t pts;
		if (!quicsy_decoder_read_frame(&dec, frame_data, &pts))
		{
			err_log("Couldn't load video frame\n");
			return 1;
		}

		static bool first_frame = true;
		if (first_frame)
		{
			glfwSetTime(0.0);
			first_frame = false;
		}

		double pt_in_seconds = pts * (double)dec.time_base.num / (double)dec.time_base.den;
		while (pt_in_seconds > glfwGetTime())
		{
			glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
		}

		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);

		// Render whatever you want
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2i(200, 200);
		glTexCoord2d(1, 0);
		glVertex2i(200 + frame_width, 200);
		glTexCoord2d(1, 1);
		glVertex2i(200 + frame_width, 200 + frame_height);
		glTexCoord2d(0, 1);
		glVertex2i(200, 200 + frame_height);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	quicsy_decoder_close(&dec);

	glfwTerminate();

	return 0;
}