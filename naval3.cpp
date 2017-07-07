#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

Mix_Music *musica = NULL; //- música de fundo
Mix_Chunk *bomba = NULL;  //- efeito sonoro mixável.
Mix_Chunk *agua = NULL;
SDL_Surface *screen = NULL;

void init(){
 SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
 //atexit(SDL_Quit);
 screen = SDL_SetVideoMode( 800, 600, 32, SDL_SWSURFACE);
 Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ); //- inicializa SDL_Mixer
 //atexit(Mix_CloseAudio);
}

void carregar(){
 musica = Mix_LoadMUS("musica.mp3");
 bomba = Mix_LoadWAV("navio.mp3");
 agua = Mix_LoadWAV("agua.mp3");
}

void inicializaTabuleiro(int tabuleiro[][5])
{
    int linha, coluna;
        for(linha=0 ; linha < 5 ; linha++ )
            for(coluna=0 ; coluna < 5 ; coluna++ )
                tabuleiro[linha][coluna]=-1;
}

void showTab(int tabuleiro[][5])
{

    int linha, coluna;


        printf("\t1 \t2 \t3 \t4 \t5");
        printf("\n");

        for(linha=0 ; linha < 5 ; linha++ ){
            printf("%d",linha+1);
            for(coluna=0 ; coluna < 5 ; coluna++ ){
                if(tabuleiro[linha][coluna]==-1){
                    printf("\t?");
                }else if(tabuleiro[linha][coluna]==0){
                    printf("\t~");
                }else if(tabuleiro[linha][coluna]==1){
                    printf("\t@");
                }

            }
            printf("\n");
        }

    }

void sortaNavios(int navios[][2]){
        srand(time(NULL)); //Atenção
        int navio, anterior;

        for(navio=0 ; navio < 3 ; navio++){
            navios[navio][0]= rand()%5;
            navios[navio][1]= rand()%5;

            for(anterior=0 ; anterior < navio ; anterior++){
                if( (navios[navio][0] == navios[anterior][0])&&(navios[navio][1] == navios[anterior][1]) )
                    do{
                        navios[navio][0]= rand()%5;
                        navios[navio][1]= rand()%5;
                    }while( (navios[navio][0] == navios[anterior][0])&&(navios[navio][1] == navios[anterior][1]) );
            }

        }
    }

void ataque(int tiro[2])
{

        printf("Insira Linha e Coluna (com espaço no meio) para dar um ataque:");
        scanf("%d %d", &tiro[0], &tiro[1]);
        tiro[0]--;
        tiro[1]--;

}

int se_acerto(int tiro[2], int navios[][2])
{
    int navio;

        for(navio=0 ; navio < 3 ; navio++){
            if( tiro[0]==navios[navio][0] && tiro[1]==navios[navio][1]){
                printf("Você acertou o tiro!!! (%d,%d)\n",tiro[0]+1,tiro[1]+1);
                Mix_PlayChannel( -1, bomba, 0 );
                return 1;
            }
        }
        Mix_PlayChannel( -1, agua, 0 );
        return 0;
    }

void dica(int tiro[2], int navios[][2], int tentativa)
{
        int linha=0,
            coluna=0,
            fila;

        for(fila=0 ; fila < 3 ; fila++){
            if(navios[fila][0]==tiro[0])
                linha++;
            if(navios[fila][1]==tiro[1])
                coluna++;
        }

        printf("\nDica %d: \nlinha %d -> %d navios\ncoluna %d -> %d navios\n",tentativa,tiro[0]+1,linha,tiro[1]+1,coluna);
}

void alteraTabuleiro(int tiro[2], int navios[][2], int tabuleiro[][5]){
        if(se_acerto(tiro,navios))
            tabuleiro[tiro[0]][tiro[1]]=1;
        else
            tabuleiro[tiro[0]][tiro[1]]=0;
    }

int main() {
        init();
        carregar();
        int tabuleiro[5][5];
        int navios[3][2];
        int tiro[2];
        int contador=0,
            acertos=0;



        printf("\n\n                    --------------------------\n");
        printf("                   |  BATALHA NAVAL - O JOGO  |\n");
        printf("                    --------------------------\n\n\n");
        printf("O jogo mostra um tabuleiro 5x5 no meio do oceano pacífico.\nSua missão é encontrar os submarinos inimigos e os explodir!\nNão tenha piedade e juntos venceremos a guerra!\nos '?' indicam posições desconhecidas,\nos '~' indicam água \ne os '@' navios explodidos.\n\n");
        printf("          Pressione enter para iniciar");
        getc(stdin);
        system("clear");
        Mix_PlayMusic( musica, -1 );

        inicializaTabuleiro(tabuleiro);
        sortaNavios(navios);

        printf("\n");

        while(acertos!=3){
            showTab(tabuleiro);
            ataque(tiro);
            system("clear");
            contador++;
            if(se_acerto(tiro, navios)){
                dica(tiro, navios, contador);
                acertos++;
            }else{
                dica(tiro, navios, contador);
            }

            alteraTabuleiro(tiro, navios, tabuleiro);
        }
                

        system("clear");
        printf("\n\n\n                        VITÓRIA! \n                        Obrigado soldado!\nGraças a você vencemos a guerra e agora podemos voltar a ter paz!\nVocê levou %d ataques para acabar com os 3 submarinos ;)\n", contador);
        showTab(tabuleiro);
    }
