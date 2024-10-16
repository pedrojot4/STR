#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <GL/glut.h>

// Variáveis para controle do jogo
int hits = 0;              // Contador de acertos
int misses = 0;           // Contador de erros
int correctKey = GLUT_KEY_DOWN;      // Tecla correta para acertar a cor
int timeLimit = 4000;                // Tempo limite para resposta (em milissegundos)
float currentColor[3] = {1.0f, 0.0f, 0.0f};  // Cor atual da bolinha
float ballPosX = 0.0f;       // Posição da bolinha x
float ballPosY = 0.0f;      // Posição da bolinha y
bool timerActive = false;            // Indica se o temporizador está ativo

// Desenha a cena, incluindo bolinha e contadores
void display(){
    glClear(GL_COLOR_BUFFER_BIT);  // Limpa o buffer de cor
    // Desenha a bolinha com a cor e posição atuais
    drawCircle(ballPosX, ballPosY, 0.1f, currentColor[0], currentColor[1], currentColor[2]);
    // Exibe os contadores de acertos e erros
    char text[45];
    sprintf(text, "Misses: %d", misses);
    drawText(text, -0.80f, 0.85f); // Posição do texto de erros
    sprintf(text, "Hits: %d", hits);
    drawText(text, -0.80f, 0.9f);  // Posição do texto de acertos

    glutSwapBuffers();  // Troca os buffers, exibindo o conteúdo desenhado
}
// Gera uma nova cor e posição aleatória para a bolinha e define a tecla correta
void generateNewColorAndPosition(){
    // Arrays para cores (RGB) e teclas correspondentes
    const float colors[4][3] = {
        {1.0f, 1.0f, 0.0f},  // Amarelo
        {1.0f, 0.0f, 0.0f},  // Vermelho
        {0.0f, 0.0f, 1.0f},  // Azul
        {0.0f, 1.0f, 0.0f}   // Verde
    };
    const int keys[4] = {
        GLUT_KEY_UP,    // Tecla correta: seta para cima
        GLUT_KEY_DOWN,  // Tecla correta: seta para baixo
        GLUT_KEY_LEFT,  // Tecla correta: seta para esquerda
        GLUT_KEY_RIGHT  // Tecla correta: seta para direita
    };
    // Gera um valor aleatório de 0 a 3 para selecionar a cor e a tecla
    int index = rand() % 4;
    // Define a cor atual e a tecla correta
    currentColor[0] = colors[index][0];
    currentColor[1] = colors[index][1];
    currentColor[2] = colors[index][2];
    correctKey = keys[index];
    // Gera posição aleatória dentro da janela de visualização (-1.0f a 1.0f)
    ballPosX = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
    ballPosY = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
}
// Desenha um círculo preenchido na tela
void drawCircle(float x, float y, float radius, float r, float g, float b) {
    glColor3f(r, g, b);  // Define a cor do círculo
    glBegin(GL_POLYGON); // Inicia o desenho de um polígono
    // Desenha um círculo com 360 segmentos
    for(int i = 0; i < 360; i++){
        float angle = i * 3.14159f / 180.0f;  // Converte o ângulo para radianos
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));  // Define vértices do círculo
    }
    glEnd();  // Finaliza o desenho do polígono
}
// Exibe texto na tela, usado para mostrar acertos e erros
void drawText(const char *text, float x, float y){
    glRasterPos2f(x, y);  // Define a posição do texto na tela
    // Exibe cada caractere do texto
    for (int i = 0; i < strlen(text); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}
// Manipulador de teclas especiais (setas do teclado)
void specialKeyHandler(int key, int x, int y){
    // Verifica se a tecla pressionada é a correta
    if(key == correctKey){
        hits++;  // Incrementa o contador de acertos
    }else{
        misses++;  // Incrementa o contador de erros
    }
    // Gera nova cor e posição após a interação
    generateNewColorAndPosition();
    glutPostRedisplay();  // Atualiza a exibição
    // Inicia o temporizador se ainda não estiver ativo
    if (!timerActive){
        timerActive = true;
        
        glutTimerFunc(timeLimit, timer, 0);  // Define o temporizador
    }
}
// Temporizador para gerar uma nova cor/posição após o tempo limite
void timer(int value){
    generateNewColorAndPosition();  // Gera nova cor e posição
    glutPostRedisplay();            // Atualiza a exibição

    timerActive = false;  // Marca o temporizador como inativo
}
// Função principal: inicializa o OpenGL e configura o loop principal do jogo
int main(int argc, char** argv){
    srand(static_cast<unsigned int>(time(0)));  // Inicializa o gerador de números aleatórios
    // Configuração inicial do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // Habilita buffers duplos e cores RGB
    glutInitWindowSize(900, 700);                 // Define o tamanho da janela
    glutCreateWindow("Reaction");                 // Cria a janela com título "Reaction"
    // Define a cor de fundo da janela (branco)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Registra as funções de callback
    glutDisplayFunc(display);           // Função de exibição
    glutSpecialFunc(specialKeyHandler);  // Função de captura de teclas especiais
    generateNewColorAndPosition();  // Gera a cor e posição iniciais
    glutMainLoop();  // Inicia o loop principal do GLUT

    return 0;
}
