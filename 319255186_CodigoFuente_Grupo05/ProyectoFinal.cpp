﻿// Inclusión de bibliotecas estándar para entrada/salida y matemáticas
#include <iostream>
#include <cmath>

// Bibliotecas para OpenGL: GLEW para extensiones, GLFW para ventanas y eventos
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Biblioteca para cargar texturas (imágenes)
#include "stb_image.h"

// Biblioteca para cargar modelos con texturas (SOIL2)
#include "SOIL2/SOIL2.h"

// GLM para operaciones matemáticas con vectores y matrices (transformaciones 3D)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Archivos personalizados para manejar shaders, cámara y modelos 3D
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



// Prototipos de funciones para manejar entrada de teclado, ratón y movimiento
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Dimensiones iniciales de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT; // Dimensiones reales del framebuffer

// Objeto cámara inicializado en (0, 0, 3) para una vista inicial
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Variables para seguimiento del ratón
GLfloat lastX = WIDTH / 4.0, lastY = HEIGHT / 4.0;
bool keys[1024]; // Arreglo para almacenar estado de teclas
bool firstMouse = true; // Evita saltos iniciales del ratón
bool estadoVentanas = false;  // Abiertas/cerradas
bool bPresionado = false;     // Control de una sola pulsación

bool estadoPuertaPrincipal = false;   // Abierta o cerrada
bool pPresionado = false;             // Control para una sola pulsación

//Variables dePuertaPrincipal
float rotacionPuertaPrincipal = 0.0f;
bool puertaPrincipalAbierta = false;
bool animandoPuertaPrincipal = false;

//Variables para botella
float rotacionBotella = 0.0f;
bool botellaCayendo = false;      // Si se está animando
bool botellaCaida = false;        // Estado actual (caída o no)
bool toggleBotella = false;       // Control de tecla


//Variables para la copa
float anguloCopa = 0.0f;
bool temblarCopa = false;
bool toggleCopa = false;

//Variables para la SeñalTv
float ruidoTimer = 0.0f;
float ruidoIntervalo = 0.1f; // velocidad de cambio entre frames
int ruidoFrameActual = 0;
bool animarRuido = false;
static bool teclaPresionada = false;

//Variables para PuertaOdin
float rotacionPuertaOdin = 0.0f;
bool puertaOdinAbierta = false;
bool animandoPuertaOdin = false;

//Variables PuertaRopero1
float rotacionRopero1 = 0.0f;      // Comienza cerrada
bool puertaAbierta = false;        // Estado actual
bool animandoRopero1 = false;      // Evita spam del toggle

//Variable PuertaRopero2
// Variable PuertaRopero2
float rotacionRopero2 = 0.0f;         // Comienza cerrada
bool puertaRopero2Abierta = false;    // Estado actual
bool animandoRopero2 = false;         // Evita spam del toggle

//Variables PuertaFer
float rotacionPuertaFer = 0.0f;
float targetRotFer = 0.0f;
bool estadoPuertaFer = false;       // Abierta o cerrada
bool togglePuertaFer = false;       // Control de tecla

//Variables Manecilla Hora
bool animarHora = false;         // Controla si está activa la animación
bool toggleHora = false;         // Evita múltiples toggles al dejar presionada la tecla
float anguloHora = 0.0f;         // Ángulo acumulado

//Variables Manecilla Minutos
bool animarMinutos = false;
bool toggleMinutos = false;
float anguloMinutos = 0.0f;



// Posición de la luz (inicialmente en el origen)
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active; // Estado de activación de la luz

// Posición del personaje y offset de la cámara para vista en tercera persona
glm::vec3 playerPosition = glm::vec3(0.0f, 0.8f, 0.0f);
glm::vec3 cameraOffset = glm::vec3(0.0f, 4.0f, 12.0f);

glm::vec3 posVF1(0.0f, 0.0f, 0.0f);
glm::vec3 posVF2(0.0f, 0.0f, 0.0f);
glm::vec3 posVF3(0.0f, 0.0f, 0.0f);
glm::vec3 posVF4(0.0f, 0.0f, 0.0f);

glm::vec3 posVD1(0.0f, 0.0f, 0.0f);
glm::vec3 posVD2(0.0f, 0.0f, 0.0f);
glm::vec3 posVD3(0.0f, 0.0f, 0.0f);
glm::vec3 posVD4(0.0f, 0.0f, 0.0f);

glm::vec3 posVI1(0.0f, 0.0f, 0.0f);
glm::vec3 posVI2(0.0f, 0.0f, 0.0f);

glm::vec3 posVT1(0.0f, 0.0f, 0.0f);
glm::vec3 posVT2(0.0f, 0.0f, 0.0f);




// Variables de transformación
float anguloPuerta = 0.0f;
float angPuertaOdin = 0.0f, angPuertaPrincipal = 0.0f, angPuertaFer = 0.0f;
float angVD1 = 0.0f, angVD2 = 0.0f, angVD3 = 0.0f, angVD4 = 0.0f;
float angVF1 = 0.0f, angVF2 = 0.0f, angVF3 = 0.0f, angVF4 = 0.0f;
float angVI1 = 0.0f, angVI2 = 0.0f;
float angVT1 = 0.0f, angVT2 = 0.0f;
float humoOffsetY = 0.0f;
float humoSpeed = 0.5f; // velocidad de subida


// Posiciones de las luces puntuales (inicialmente en el origen)
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

// Vértices de un cubo para representar las fuentes de luz (posición + normal)
float vertices[] = {
    // Cara trasera
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // Cara frontal
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    // Cara izquierda
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    // Cara derecha
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     // Cara inferior
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     // Cara superior
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Color inicial de la luz (apagada)
glm::vec3 Light1 = glm::vec3(0);

// Variables para calcular el tiempo entre fotogramas (movimiento suave)
GLfloat deltaTime = 0.0f; // Tiempo entre el fotograma actual y el anterior
GLfloat lastFrame = 0.0f; // Tiempo del último fotograma

// Estructura de partícula
struct Humo {
    glm::vec3 posicion;
    float tiempo;
    float velocidad;
    float escala;
};

// Variables globales
std::vector<Humo> particulasHumo;
glm::vec3 origenHumo = glm::vec3(19.75f, 3.85f, 17.85f); // Pico de la tetera
int totalParticulas = 15;

// Variables para animar un pájaro que se posa y luego vuela
bool pajaroVolando = false;
bool togglePajaro = false;
float pajaroAngulo = 0.0f;
float pajaroAltura = 0.0f;
glm::vec3 arbustoBase = glm::vec3(5.943f, 1.692f, 23.127f);
float radioVuelo = 3.0f;


// Función principal
int main() {


    // Inicializa GLFW para gestionar ventanas y eventos
    glfwInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Crea una ventana de 800x600 píxeles con el título "Fuentes de luz"
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ProyectoFinal", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE; // Termina si no se pudo crear la ventana
    }

    // Establece la ventana como el contexto actual de OpenGL
    glfwMakeContextCurrent(window);

    // Obtiene las dimensiones reales del framebuffer
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Asocia funciones de callback para teclado y ratón
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Configura GLEW para usar un enfoque moderno en extensiones
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE; // Termina si no se pudo inicializar GLEW
    }

    // 1. Crear ventana GLFW
// 2. glfwMakeContextCurrent(window);
// 3. glewInit()
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Configura parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    


    // Define el área de renderizado (viewport) según las dimensiones de la ventana
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Carga los shaders para objetos iluminados y fuentes de luz
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Carga los modelos 3D (casa y personaje)
    // Cargar modelos
    Model Casa((char*)"Models/animables/Casita/casita.obj"); // Modelo de la casa
    Model PuertaPrincipal((char*)"Models/animables/PuertaPrincipal/PuertaPrincipal.obj");//Modelo de la puerta principal
    Model PuertaOdin((char*)"Models/animables/PuertaOdin/PuertaOdin.obj");//Modelo de la puerta de Odin
    Model PuertaFer((char*)"Models/animables/PuertaFer/PuertaFer.obj");//Modelo de la puerta de fer
    Model Copa((char*)"Models/animables/Copa/Copa.obj");//Modelo de la copa de la mesa
    Model SenalTV((char*)"Models/animables/RuidoTV/RuidoTv.obj");//Modelo de la señal de tv
    Model Botella((char*)"Models/animables/Botella/Botella.obj");//Modelo de la botella de la mesa
    Model PuertaRopero1((char*)"Models/animables/PuertaRopero1/PuertaRopero1.obj");//Puerta Ropero 1
    Model PuertaRopero2((char*)"Models/animables/PuertaRopero2/PuertaRopero2.obj");//Puerta Ropero 1
    Model Personaje((char*)"Models/snoopy.obj"); //Modelo del personaje
    Model Reloj((char*)"Models/animables/Reloj/Reloj.obj"); //Modelo del personaje
    Model ManecillaHora((char*)"Models/animables/ManecillaHora/ManecillaHora.obj"); //Modelo del personaje
    Model ManecillaMinutos((char*)"Models/animables/ManecillaMinutos/ManecillaMinutos.obj"); //Modelo del personaje
    Model pajaro((char*)"Models/pajaro_azul.obj");

    Model vd1((char*)"Models/vd1.obj");
    Model vd2((char*)"Models/vd2.obj");
    Model vd3((char*)"Models/vd3.obj");
    Model vd4((char*)"Models/vd4.obj");

    Model vf1((char*)"Models/vf1.obj");
    Model vf2((char*)"Models/vf2.obj");
    Model vf3((char*)"Models/vf3.obj");
    Model vf4((char*)"Models/vf4.obj");

    Model vi1((char*)"Models/vi1.obj");
    Model vi2((char*)"Models/vi2.obj");

    Model vt1((char*)"Models/vt1.obj");
    Model vt2((char*)"Models/vt2.obj"); 
    
    Model esfera((char*)"Models/esfera.obj");

    for (int i = 0; i < totalParticulas; ++i) {
        Humo p;
        p.posicion = origenHumo;
        p.tiempo = static_cast<float>(rand() % 100) / 100.0f;
        p.velocidad = 0.3f + static_cast<float>(rand() % 30) / 100.0f;
        p.escala = 0.8f;
        particulasHumo.push_back(p);
    }


    //TEXTURAS PARA SENALTV
    GLuint ruidoFrames[6];
    ruidoFrames[0] = TextureFromFile("ruido_1.png", "Models/animables/RuidoTV/");
    ruidoFrames[1] = TextureFromFile("ruido_2.png", "Models/animables/RuidoTV/");
    ruidoFrames[2] = TextureFromFile("ruido_3.png", "Models/animables/RuidoTV/");
    ruidoFrames[3] = TextureFromFile("ruido_4.png", "Models/animables/RuidoTV/");
    ruidoFrames[4] = TextureFromFile("ruido_5.png", "Models/animables/RuidoTV/");
    ruidoFrames[5] = TextureFromFile("ruido_6.png", "Models/animables/RuidoTV/");
    glBindTexture(GL_TEXTURE_2D, ruidoFrames[ruidoFrameActual]);


    // Configura los buffers para los vértices del cubo (usado para luces)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Atributo de posición (3 componentes: x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Atributo de normal (3 componentes para iluminación)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configura las unidades de textura en el shader de iluminación
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    // Define la matriz de proyección (perspectiva)
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Bucle principal del juego
    while (!glfwWindowShouldClose(window)) {
        // Calcula el tiempo entre fotogramas para movimiento suave
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

     


        // Procesa eventos de teclado y ratón
        glfwPollEvents();
        DoMovement(); // Actualiza posiciones del personaje y luces

        // Limpia los buffers de color y profundidad con un fondo gris oscuro
        glClearColor(0.52f, 0.80f, 0.92f, 1.0f);//Color del fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activa pruebas de profundidad y mezcla para transparencias
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Usa el shader de iluminación para los objetos
        lightingShader.Use();

        // Configura texturas difusa y especular
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1);

        // Pasa la posición de la cámara al shader
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Configura la luz direccional (como un sol)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);

        // Configura la primera luz puntual (con color dinámico)
        pointLightPositions[0] = glm::vec3(0.0f, 5.0f, 0.0f);
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x)); // Color pulsante
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // Configura las otras luces puntuales (desactivadas)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

        // Configura la luz de foco (spotlight) que sigue la cámara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // Configura la propiedad de brillo del material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Crea la matriz de vista para la cámara en tercera persona
        glm::mat4 view;
        glm::vec3 newCamPos = playerPosition + cameraOffset; // Posición de la cámara relativa al personaje
        view = glm::lookAt(newCamPos, playerPosition, glm::vec3(0.0f, 1.0f, 0.0f)); // Mira al personaje



        // Obtiene las ubicaciones de las matrices en el shader
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pasa las matrices al shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (auto& p : particulasHumo) {
            p.tiempo += deltaTime;
            p.posicion.y += p.velocidad * deltaTime;
            p.posicion.x += sin(p.tiempo * 3.0f) * 0.01f;
            p.posicion.z += cos(p.tiempo * 2.0f) * 0.01f;
            p.escala += deltaTime * 0.08f;

            if (p.posicion.y > origenHumo.y + 2.0f) {
                p.posicion = origenHumo;
                p.tiempo = 0;
                p.escala = 0.1f;
            }
        }

        glm::vec3 posPajaro;
        if (pajaroVolando) {
            pajaroAngulo += deltaTime * 1.5f;
            if (pajaroAngulo > 2 * M_PI) pajaroAngulo -= 2 * M_PI;
            pajaroAltura += deltaTime * 0.5f;
            if (pajaroAltura > 2.0f) pajaroAltura = 2.0f;

            posPajaro.x = arbustoBase.x + cos(pajaroAngulo) * radioVuelo;
            posPajaro.z = arbustoBase.z + sin(pajaroAngulo) * radioVuelo;
            posPajaro.y = arbustoBase.y + pajaroAltura;
        }
        else {
            posPajaro = arbustoBase;
            pajaroAltura = 0.0f;
            pajaroAngulo = 0.0f;
        }


        // Dibuja la casa
        glm::mat4 model(1);
        glEnable(GL_BLEND); // Activa la transparencia
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Casa.Draw(lightingShader); // Renderiza el modelo de la casa

        // Dibuja el personaje
        model = glm::mat4(1.0f);
        model = glm::translate(model, playerPosition); // Posiciona el personaje
        model = glm::scale(model, glm::vec3(0.7f)); // Escala el modelo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Personaje.Draw(lightingShader); // Renderiza el modelo del personaje


        // =======================
        //		PuertaPrincipal
        // =======================
        // Animar rotación progresiva de PuertaPrincipal
        // ANIMACIÓN
        if (puertaPrincipalAbierta && rotacionPuertaPrincipal > -120.0f) {
            rotacionPuertaPrincipal -= 50.0f * deltaTime;
            if (rotacionPuertaPrincipal < -120.0f)
                rotacionPuertaPrincipal = -120.0f;
        }
        else if (!puertaPrincipalAbierta && rotacionPuertaPrincipal < 0.0f) {
            rotacionPuertaPrincipal += 50.0f * deltaTime;
            if (rotacionPuertaPrincipal > 0.0f)
                rotacionPuertaPrincipal = 0.0f;
        }

        // RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivotePrincipal = glm::vec3(-4.068352699279785f, 4.48995304107666f, 21.405784606933594f);
        model = glm::translate(model, pivotePrincipal);
        model = glm::rotate(model, glm::radians(rotacionPuertaPrincipal), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, -pivotePrincipal);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PuertaPrincipal.Draw(lightingShader);


        // =======================
        //		PuertaOdin
        // =======================
        // ANIMACIÓN
        if (puertaOdinAbierta && rotacionPuertaOdin > -120.0f) {
            rotacionPuertaOdin -= 50.0f * deltaTime;
            if (rotacionPuertaOdin < -120.0f)
                rotacionPuertaOdin = -120.0f;
        }
        else if (!puertaOdinAbierta && rotacionPuertaOdin < 0.0f) {
            rotacionPuertaOdin += 50.0f * deltaTime;
            if (rotacionPuertaOdin > 0.0f)
                rotacionPuertaOdin = 0.0f;
        }

        // RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivoteOdin = glm::vec3(1.004035472869873f, 4.038388252258301f, -2.5722479820251465f);
        model = glm::translate(model, pivoteOdin);
        model = glm::rotate(model, glm::radians(rotacionPuertaOdin), glm::vec3(0.0f, 1.0f, 0.0f)); // Eje Y
        model = glm::translate(model, -pivoteOdin);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PuertaOdin.Draw(lightingShader);


        // =======================
        //		PuertaFer
        // =======================
        // ANIMACION
        targetRotFer = estadoPuertaFer ? -120.0f : 0.0f;

        // Interpolación hacia el objetivo
        float velocidadFer = 50.0f * deltaTime;
        if (rotacionPuertaFer < targetRotFer)
            rotacionPuertaFer = glm::min(rotacionPuertaFer + velocidadFer, targetRotFer);
        else if (rotacionPuertaFer > targetRotFer)
            rotacionPuertaFer = glm::max(rotacionPuertaFer - velocidadFer, targetRotFer);

        // RENDERIZADO
        glm::mat4 modelFer = glm::mat4(1.0f);
        glm::vec3 pivoteFer = glm::vec3(-9.896177291870117f, 4.038389205932617f, 6.952088832855225f);
        modelFer = glm::translate(modelFer, pivoteFer);
        modelFer = glm::rotate(modelFer, glm::radians(rotacionPuertaFer), glm::vec3(0.0f, 1.0f, 0.0f));
        modelFer = glm::translate(modelFer, -pivoteFer);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFer));
        PuertaFer.Draw(lightingShader);


        // =======================
        //			Copa
        // =======================
        //ANIMACION
        if (temblarCopa) {
            anguloCopa = sin(glfwGetTime() * 10.0f) * 5.0f; // Oscila suavemente entre -5° y +5°
        }

        //RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivoteCopa = glm::vec3(-0.959242f, 3.597939f, -17.372900f);
        model = glm::translate(model, pivoteCopa);
        model = glm::rotate(model, glm::radians(anguloCopa), glm::vec3(0.0f, 0.0f, 1.0f)); // Gira sobre eje Z (ajustable)
        model = glm::translate(model, -pivoteCopa);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Copa.Draw(lightingShader);

        // =======================
        //		SeñalTv
        // =======================
        //RENDERIZADO
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ruidoFrames[ruidoFrameActual]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3()); // Usa la posición adecuada
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        SenalTV.Draw(lightingShader);


        // =======================
        //			Botella
        // =======================
        //ANIMACION
        if (botellaCayendo) {
            float velocidad = 45.0f * deltaTime;

            if (botellaCaida && rotacionBotella < 90.0f) {
                rotacionBotella += velocidad;
                if (rotacionBotella >= 90.0f) {
                    rotacionBotella = 90.0f;
                    botellaCayendo = false;
                }
            }
            else if (!botellaCaida && rotacionBotella > 0.0f) {
                rotacionBotella -= velocidad;
                if (rotacionBotella <= 0.0f) {
                    rotacionBotella = 0.0f;
                    botellaCayendo = false;
                }
            }
        }

        //RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivoteBotella = glm::vec3(-2.303459f, 4.206707f, -17.304603f);
        model = glm::translate(model, pivoteBotella);
        model = glm::rotate(model, glm::radians(rotacionBotella), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -pivoteBotella);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Botella.Draw(lightingShader);


        // =======================
        //		PuertaRopero1
        // =======================
        //ANIMACION
        if (puertaAbierta && rotacionRopero1 > -120.0f) {
            rotacionRopero1 -= 50.0f * deltaTime;
            if (rotacionRopero1 < -120.0f)
                rotacionRopero1 = -120.0f;
        }
        else if (!puertaAbierta && rotacionRopero1 < 0.0f) {
            rotacionRopero1 += 50.0f * deltaTime;
            if (rotacionRopero1 > 0.0f)
                rotacionRopero1 = 0.0f;
        }

        // RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivoteRopero1 = glm::vec3(15.191308f, 4.863689f, -8.121943f);

        model = glm::translate(model, pivoteRopero1);
        model = glm::rotate(model, glm::radians(rotacionRopero1), glm::vec3(0.0f, -1.0f, 0.0f)); // Eje Y
        model = glm::translate(model, -pivoteRopero1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PuertaRopero1.Draw(lightingShader);


        // =======================
        //		PuertaRopero2
        // =======================
        // ANIMACIÓN
        if (puertaRopero2Abierta && rotacionRopero2 > -120.0f) {
            rotacionRopero2 -= 50.0f * deltaTime;
            if (rotacionRopero2 < -120.0f)
                rotacionRopero2 = -120.0f;
        }
        else if (!puertaRopero2Abierta && rotacionRopero2 < 0.0f) {
            rotacionRopero2 += 50.0f * deltaTime;
            if (rotacionRopero2 > 0.0f)
                rotacionRopero2 = 0.0f;
        }
        //RENDERIZADO
        model = glm::mat4(1.0f);
        glm::vec3 pivoteRopero2 = glm::vec3(20.6259765625f, 4.822100639343262f, -8.121943473815918f);
        model = glm::translate(model, pivoteRopero2);
        model = glm::rotate(model, glm::radians(rotacionRopero2), glm::vec3(0.0f, 1.0f, 0.0f)); // Eje Y
        model = glm::translate(model, -pivoteRopero2);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PuertaRopero2.Draw(lightingShader);


        // =======================
        //			Personaje
        // =======================
        //RENDERIZADO
        model = glm::mat4(1.0f);
        model = glm::translate(model, playerPosition);
        model = glm::scale(model, glm::vec3(0.7f)); // Ajusta el tamaño del modelo
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Personaje.Draw(lightingShader);


        // ================================
        //		Cuerpo del Reloj
        // ================================
        //RENDERIZADO
        glm::mat4 modelReloj = glm::mat4(1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelReloj));
        Reloj.Draw(lightingShader);


        // ================================
        //		Manecilla Hora
        // ================================
        //ANIMACION
        if (animarHora) {
            anguloHora += deltaTime * 6.0f;  // 6 por segundo  1 vuelta en 60 segundos
            if (anguloHora >= 360.0f) anguloHora -= 360.0f;
        }

        //RENDERIZADO
        glm::mat4 modelHora = glm::mat4(1.0f);
        glm::vec3 pivoteHora = glm::vec3(-9.090096001747589f, 4.95666558468782f, -8.980611363899742f);
        modelHora = glm::translate(modelHora, pivoteHora);
        modelHora = glm::rotate(modelHora, glm::radians(anguloHora), glm::vec3(-1.0f, 0.0f, 0.0f));
        modelHora = glm::translate(modelHora, -pivoteHora);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHora));
        ManecillaHora.Draw(lightingShader);


        // ================================
        //		Manecilla Minutos
        // ================================
        //ANIMACION
        if (animarMinutos) {
            anguloMinutos += deltaTime * 72.0f;  // Coordinado con ManecillaHora (1 vuelta en 5s)
            if (anguloMinutos >= 360.0f) anguloMinutos -= 360.0f;
        }

        //RENDERIZADO
        glm::mat4 modelMin = glm::mat4(1.0f);
        glm::vec3 pivoteMin = glm::vec3(-9.107255224796003f, 4.902407769451714f, -8.968057866693192f);
        modelMin = glm::translate(modelMin, pivoteMin);
        modelMin = glm::rotate(modelMin, glm::radians(anguloMinutos), glm::vec3(-1.0f, 0.0f, 0.0f)); // Eje igual que en Maya
        modelMin = glm::translate(modelMin, -pivoteMin);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMin));
        ManecillaMinutos.Draw(lightingShader);

        // =======================
        // FIN DE RENDERIZADO DE OBJETOS
        // =======================

        glDisable(GL_BLEND);  //Desactiva el canal alfa 
        glBindVertexArray(0);


        // VD1 a VD4
        model = glm::mat4(1.0f);
        model = glm::translate(model, posVD1);
        model = glm::rotate(model, glm::radians(angVD1), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vd1.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVD2);
        model = glm::rotate(model, glm::radians(angVD2), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vd2.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVD3);
        model = glm::rotate(model, glm::radians(angVD3), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vd3.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVD4);
        model = glm::rotate(model, glm::radians(angVD4), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vd4.Draw(lightingShader);

        // VF1 a VF4
        model = glm::mat4(1.0f);
        model = glm::translate(model, posVF1);
        model = glm::rotate(model, glm::radians(angVF1), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vf1.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVF2);
        model = glm::rotate(model, glm::radians(angVF2), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vf2.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVF3);
        model = glm::rotate(model, glm::radians(angVF3), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vf3.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVF4);
        model = glm::rotate(model, glm::radians(angVF4), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vf4.Draw(lightingShader);

        // VI1 y VI2
        model = glm::mat4(1.0f);
        model = glm::translate(model, posVI1);
        model = glm::rotate(model, glm::radians(angVI1), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vi1.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVI2);
        model = glm::rotate(model, glm::radians(angVI2), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vi2.Draw(lightingShader);

        // VT1 y VT2
        model = glm::mat4(1.0f);
        model = glm::translate(model, posVT1);
        model = glm::rotate(model, glm::radians(angVT1), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vt1.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, posVT2);
        model = glm::rotate(model, glm::radians(angVT2), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vt2.Draw(lightingShader);

  

        // MATRIZ DE TRANSFORMACIÓN DEL HUMO
        glm::mat4 modelHumo = glm::mat4(1.0f);
        modelHumo = glm::translate(modelHumo, glm::vec3(0.0f, 0.0f + humoOffsetY, 0.0f));
        modelHumo = glm::scale(modelHumo, glm::vec3(0.4f));

        // ENVÍA LA MATRIZ AL SHADER
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHumo));
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE); // Desactiva escritura en el Z-buffer

        

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        for (auto& p : particulasHumo) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, p.posicion);
            model = glm::scale(model, glm::vec3(p.escala));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
            esfera.Draw(lightingShader);
        }

        // Renderizar el pájaro
        glm::mat4 modelPj = glm::mat4(1.0f);
        modelPj = glm::translate(modelPj, posPajaro);
        modelPj = glm::scale(modelPj, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPj));
        pajaro.Draw(lightingShader);


        // Usa el shader para las fuentes de luz
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Pasa las matrices al shader de la lámpara
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibuja las fuentes de luz como cubos pequeños
        for (GLuint i = 0; i < 4; i++) {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Cubo pequeño
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36); // Dibuja el cubo
        }
        glBindVertexArray(0);

        // Intercambia los buffers para mostrar el fotograma renderizado
        glfwSwapBuffers(window);
    }

    // Libera los recursos de GLFW y termina el programa
    glfwTerminate();
    return 0;
}

// Función para manejar el movimiento del personaje y la luz
void DoMovement() {
    float speed = 20.0f * deltaTime; // Velocidad ajustada al tiempo

    // Movimiento del personaje con teclas W, S, A, D o flechas
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_UP])
        playerPosition.z -= speed; // Mueve hacia adelante
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_DOWN])
        playerPosition.z += speed; // Mueve hacia atrás
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_LEFT])
        playerPosition.x -= speed; // Mueve a la izquierda
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_RIGHT])
        playerPosition.x += speed; // Mueve a la derecha

    // Movimiento manual de la primera luz puntual
    if (keys[GLFW_KEY_T])
        pointLightPositions[0].x += 0.01f; // Derecha
    if (keys[GLFW_KEY_G])
        pointLightPositions[0].x -= 0.01f; // Izquierda
    if (keys[GLFW_KEY_F])
        pointLightPositions[0].y += 0.01f; // Arriba
    if (keys[GLFW_KEY_H])
        pointLightPositions[0].y -= 0.01f; // Abajo
    if (keys[GLFW_KEY_U])
        pointLightPositions[0].z -= 0.1f; // Adelante
    if (keys[GLFW_KEY_J])
        pointLightPositions[0].z += 0.01f; // Atrás
    //PuertaPrincipal
    if (keys[GLFW_KEY_P] && !animandoPuertaPrincipal) {
        puertaPrincipalAbierta = !puertaPrincipalAbierta;
        animandoPuertaPrincipal = true;
    }
    if (!keys[GLFW_KEY_P]) {
        animandoPuertaPrincipal = false;
    }

    //Botella

    if (keys[GLFW_KEY_B] && !toggleBotella) {
        botellaCaida = !botellaCaida;        // Cambia el estado
        botellaCayendo = true;               // Inicia animación
        toggleBotella = true;
    }
    if (!keys[GLFW_KEY_B]) {
        toggleBotella = false;
    }


    //Copa
    if (keys[GLFW_KEY_C] && !toggleCopa) {
        temblarCopa = !temblarCopa;   // Cambia el estado
        toggleCopa = true;
    }
    if (!keys[GLFW_KEY_C]) {
        toggleCopa = false;
    }


    //Señal
    if (keys[GLFW_KEY_R] && !teclaPresionada) {
        animarRuido = !animarRuido;
        teclaPresionada = true;
    }
    if (!keys[GLFW_KEY_R]) {
        teclaPresionada = false;
    }

    //PuertaRopero 1
    if (keys[GLFW_KEY_M] && !animandoRopero1) {
        puertaAbierta = !puertaAbierta;   // Cambia el estado
        animandoRopero1 = true;           // Evita múltiples toggles
    }
    if (!keys[GLFW_KEY_M]) {
        animandoRopero1 = false;          // Se suelta la tecla
    }

    // PuertaRopero2
    if (keys[GLFW_KEY_N] && !animandoRopero2) {
        puertaRopero2Abierta = !puertaRopero2Abierta;  // Cambia el estado
        animandoRopero2 = true;                         // Evita múltiples toggles
    }
    if (!keys[GLFW_KEY_N]) {
        animandoRopero2 = false;                        // Se suelta la tecla
    }

    //PuertaOdin
    if (keys[GLFW_KEY_O] && !animandoPuertaOdin) {
        puertaOdinAbierta = !puertaOdinAbierta;
        animandoPuertaOdin = true;
    }
    if (!keys[GLFW_KEY_O]) {
        animandoPuertaOdin = false;
    }


    //PuertaFer
    if (keys[GLFW_KEY_F] && !togglePuertaFer) {
        estadoPuertaFer = !estadoPuertaFer;
        togglePuertaFer = true;
    }
    if (!keys[GLFW_KEY_F]) {
        togglePuertaFer = false;
    }

    //  Manecilla Hora 
    if (keys[GLFW_KEY_I] && !toggleHora) {
        animarHora = !animarHora;
        toggleHora = true;
    }
    if (!keys[GLFW_KEY_I]) {
        toggleHora = false;
    }

    // Manecilla Minutos 
    if (keys[GLFW_KEY_U] && !toggleMinutos) {
        animarMinutos = !animarMinutos;
        toggleMinutos = true;
    }
    if (!keys[GLFW_KEY_U]) {
        toggleMinutos = false;
    }
    if (keys[GLFW_KEY_L] && !togglePajaro) {
        pajaroVolando = !pajaroVolando;
        togglePajaro = true;
    }
    if (!keys[GLFW_KEY_L]) {
        togglePajaro = false;
    }

}

// Función para manejar eventos de teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Cierra la ventana al presionar ESC
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Actualiza el estado de las teclas (presionada o liberada)
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    // Alterna el estado de la luz al presionar ESPACIO
    if (keys[GLFW_KEY_SPACE]) {
        active = !active;
        if (active) {
            Light1 = glm::vec3(1.0f, 1.0f, 0.0f); // Luz amarilla
        }
        else {
            Light1 = glm::vec3(0); // Luz apagada
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        bPresionado = true;
        if (bPresionado) {
            estadoVentanas = !estadoVentanas;

            if (estadoVentanas) {
                // Abrir
                posVF1.x += 3.5f;
                posVF2.x -= 3.5f;
                posVF3.x += 4.8f;
                posVF4.x -= 4.8f;

                posVD1.z -= 4.7f;
                posVD2.z += 4.7f;
                posVD3.z -= 4.8f;
                posVD4.z += 4.8f;

                posVT1.x -= 5.5f;
                posVT2.x += 5.5f;

                posVI1.z += 9.1f;
                posVI2.z -= 9.1f;
            }
            else {
                // Cerrar (regresar)
                posVF1.x -= 3.5f;
                posVF2.x += 3.5f;
                posVF3.x -= 4.8f;
                posVF4.x += 4.8f;

                posVD1.z += 4.7f;
                posVD2.z -= 4.7f;
                posVD3.z += 4.8f;
                posVD4.z -= 4.8f;

                posVT1.x += 5.5f;
                posVT2.x -= 5.5f;

                posVI1.z -= 9.1f;
                posVI2.z += 9.1f;
            }

            bPresionado = false;
        }

 

}

// Función para manejar el movimiento del ratón (control de cámara)
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    // Evita saltos iniciales del ratón
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    // Calcula el desplazamiento del ratón
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // Invertido para movimiento natural
    lastX = xPos;
    lastY = yPos;

    // Ajusta sensibilidad del ratón
    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Ángulos de rotación para la cámara (yaw y pitch)
    static float yaw = -90.0f;
    static float pitch = 10.0f;
    yaw += xOffset;
    pitch += yOffset;

    // Limita el ángulo de pitch para evitar volteretas
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < 5.0f) pitch = 5.0f; // Evita cámara demasiado baja

    // Calcula la posición de la cámara en coordenadas esféricas
    float radius = 6.0f; // Distancia de la cámara al personaje
    float camX = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    float camY = radius * sin(glm::radians(pitch));
    float camZ = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    // Asegura que la cámara no baje demasiado
    if ((playerPosition.y + camY) < 1.0f)
        camY = 1.0f - playerPosition.y;

    // Actualiza el offset de la cámara
    cameraOffset = glm::vec3(camX, camY, camZ);
}