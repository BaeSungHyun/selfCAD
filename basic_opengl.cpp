void CstructureView::triangle() {
	// NDC (Normalized Device Coordinates) - Once your vertex coordinates have been
	// processed in the vertex shader, they should be in NDC which is a small place
	// where x, y, z vary from -1 to 1. Any coordinates that fall outside this range
	// will be discarded/clipped and won't be visible on your screen

	// NDC coordniates will then be transformed to screen-space coordinates via the 
	// viewport transform using the data you provided with 'glViewport'. The resulting
	// screen-space coordinates are then transformed to fragments as inputs to your
	// fragment shader. 
	float vertices[] = { -0.5f, -0.5f, 0.0f,
						0.5f, -0.5f, 0.0f,
						0.0f, 0.5f, 0.0f
	};
	// This is Vertex Buffer Object. There are many other objects.

	// 1. Send vertices to vertex shader - the vertex shader can process as much 
	// vertices as we tell it to from its memory. We manage this memory by VBO. 
	// Advantage is that we can send large batches of data all at once to the graphic
	// card. Like any object in OpenGL, this buffer has a unique ID corresponding
	// to that buffer.
	unsigned int TriangleVBO;
	glGenBuffers(1, &TriangleVBO);

	// 2. There are many types of buffer objects and the buffer type of a vertex 
	// object is GL_ARRAY_BUFFER. OpenGL allows us to bind to several buffers at once
	// as long as they have a different buffer type.
	glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);

	// 3. Any buffer calls we make on the GL_ARRAY_BUFFER will be used to configure
	// the currently bound buffer, which is VBO. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// GL_STREAM _DRAW : the data is set only once and used by GPU at most few times
	// GL_STATIC_DRAW : the data is set only once and used many times
	// GL_DYNAMIC_DRAW : the data is changed a lot and used many times

	// 4. Send VBO to vertex shader. 
	// declare all the input vertex atributes in the vertex shader with 'in' keyword
	// specifically set the location of the input variable via 'layout (location=0)'
	// To set the ouput of the vertex shader we have to assign the position data
	// to the predefined gl_Position variable which is a vec4 behind the scenes. 
	const char* TriangleVertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";
	// It's much compex in real application. First have to transform the input data
	// to coordinates that fall within OpenGL's visible region.

	// 5. To use shader, dynamically compile it at run-time from its source code. 
	// create a shader object, again reference by an ID.
	unsigned int TriangleVertexShader;
	TriangleVertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 6. attach the shader source code to the shader object and compile the shader
	glShaderSource(TriangleVertexShader, 1, &TriangleVertexShaderSource, NULL);
	// second arguemnt : how many strings are passeing as source code
	glCompileShader(TriangleVertexShader);

	// 7. (Optional) Check if compilation was successful after glCompileShader
	int success;
	char infoLog[512];
	glGetShaderiv(TriangleVertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(TriangleVertexShader, 512, NULL, infoLog);
		MessageBox(NULL, (LPCTSTR)infoLog, NULL);
	}

	// 8. Fragement Shader - all about calcualting the color output of your pixels
	// requires one output variable and that is a vector of size 4 that defines the
	// final color output that we should calculate ourselves. 
	const char* TriangleFragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2, 1.0f);\n"
		"}";
	// rest same with vertex shader
	unsigned int TriangleFragmentShader;
	TriangleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(TriangleFragmentShader, 1, &TriangleFragmentShaderSource, NULL);
	glCompileShader(TriangleFragmentShader);

	glGetShaderiv(TriangleFragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(TriangleFragmentShader, 512, NULL, infoLog);
		MessageBox(NULL, (LPCTSTR)infoLog, NULL);
	}

	// 9. link both shader objects into a shader program that we can use for rendering
	// Shader program object is final linked version of multiple shaders combined. 
	// To use recently compiled shaders link them to a shader program object and then
	// activate this shader program when rendering objects. 
	// Ouputs of each shader to the inputs of the next shader. This is also where 
	// you'll get linkning errors if your outputs and inputs do not match.
	unsigned int TriangleShaderProgram;
	TriangleShaderProgram = glCreateProgram();
	glAttachShader(TriangleShaderProgram, TriangleVertexShader);
	glAttachShader(TriangleShaderProgram, TriangleFragmentShader);
	glLinkProgram(TriangleShaderProgram); // attach -> link

	// 10. Check for errors during linking
	glGetProgramiv(TriangleShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(TriangleShaderProgram, 512, NULL, infoLog);
	}

	// 11. Program object that we can activate by calling glUseProgram
	// glUseProgram(TriangleShaderProgram);

	// 12. Don't forget to delete the shader objects once we've linked then into 
	// the program object. We no longer need them anymore.
	glDeleteShader(TriangleVertexShader);
	glDeleteShader(TriangleFragmentShader);
	// We sent the input vertex data to the GPU and instructed the GPU how it should
	// process the vertex data within a vertex and fragment shader. OpenGL does not 
	// know how it should interpret the vertex data in memory and how it should 
	// connect the vertex data to the vertex shader's attributes. 

	// 13. Linking Vertex Attributes
	// we have to manully specify what part of our input data goies to which 
	// vertex attribute in the vertex shader. We have to specify how OpenGL should
	// interpret the vertex data before rendering.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 0 : which vertex attribute we want to configure. 'layout (location = 0)'
	// 3 : size of the vertex attribute. 
	// GL_FLOAT : vec* in GLSL
	// GL_FALSE : data normalization. For integers?
	// 3*sizeof(float) : stride 
	// (void*)0 : offset of where the position data begins in the buffer
	// Each vertex attribute takes its data from memory managed by a VBO and which 
	// VBO it takes its data from (you can have multiple VBOs) is determined by the
	// VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer. 

	// Enable the vertex attribute, giving the vertex attribute location as its 
	// argument; vertex attributes are disables by default.
	glEnableVertexAttribArray(0); // needed ? 

	glUseProgram(TriangleShaderProgram); // needed ?

	// 14. VAO - can be bound just like a vertex buffer object and any subsequent
	// vertex attribute calls from that point on will be stored inside the VAO. 
	// (Core OpenGL requires that we use a VAO so it knows what to do with our 
	// vertex inputs. If we fail to bind a VAO, won't draw anything. )
	unsigned int TriangleVAO;
	glGenVertexArrays(1, &TriangleVAO);

	glBindVertexArray(TriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glUseProgram(TriangleShaderProgram);
	glBindVertexArray(TriangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// VAO makes switching between different vertex data and attribute configuratinos
	// as easy as binding a different VAO. All the state we just set is stored inside
	// the VAO. 

	// To use VAO all you have to do is bind the VAO using glBindVertexArray. 
	// As we want to draw an object, we simply bind the VAO with the prefereed 
	// settings before drawing the object and thats it. 
}



// or - use Element Buffer Object (EBO) to specify the order 
float vertices[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indieces, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glBindVertexArray(0);

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);


void CstructureView::triangle() {
	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0"; // null terminated string

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		MessageBoxA(NULL, (LPCSTR)infoLog, NULL, MB_OK);
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		MessageBoxA(NULL, (LPCSTR)infoLog, NULL, MB_OK);
	}

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		MessageBoxA(NULL, (LPCSTR)infoLog, NULL, MB_OK);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f, // right
		0.0f, 0.5f, 0.0f // top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the VAO first, then bind and set vertex buffer(s) and configure vertex attribute(s)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer
	// object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rerely happens. Modifying
	// other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly
	// necessary.
	glBindVertexArray(0);

	// draw triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do 
	// so to keep things a bit moreorganized
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); // no need to unbind it every time
}