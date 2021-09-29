#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>  //�E�A���t�@�x�b�g���ǂ������ׂ�֐� isalpha()���g�p���邽��

#define MAZE_NUM 15 //�E���H�͈̔�

//�E�e�L�����N�^�[�i�I�u�W�F�N�g�j�ݒ�
typedef enum{
    OBJ_WALL,   //�E��
    OBJ_SPACE,  //�E��
    OBJ_PLAYER, //�E�v���C���[
    OBJ_GOAL,   //�E�S�[��
}Object ;

//�E���W�\����
typedef struct _Point{
    int x ;
    int y ;
}Point ;

//�E�S�I�u�W�F�N�g�̏�Ԃ�\���\����
typedef struct _State{
    Object mObjects[MAZE_NUM][MAZE_NUM] ;   //�E�I�u�W�F�N�g�̔z�u
    Point mPlayerPoint ;    //�E�v���C���[�̏ꏊ
    Point mGoalPoint ;      //�E�S�[���̏ꏊ
    bool mIsClear ;         //�E�N���A����
}State ;

//�E�e��֐��錾
void setObject(State* s);   //�E�S�I�u�W�F�N�g�ݒ�
void drawObject(State* s);  //�E�S�I�u�W�F�N�g�\��
char getInput() ;           //�E���͎擾
void movePlayer(State* s, Point vector);//�E�v���C���[�̈ړ�
void update(State* s, char input);      //�E�I�u�W�F�N�g�X�V


/*****************************
���C���֐��ł�
******************************/
int main(void){

    //�E�\���̍쐬
    State* state = (State*) malloc(sizeof(State)) ;

    //�E�ݒ�
    setObject(state) ;

    //�E���C�����[�v
    while(state->mIsClear == false){

        //�E�`��
        drawObject(state) ;
        printf("�S�[��G��ڎw���Ă��������B") ;
        printf("������@ w:�� z:�� a:�� s:�� \n") ;
        printf("input key: ") ;

        //�E���͎擾
        char inputKey ;
        inputKey = getInput() ;

        //�E�X�V
        update(state, inputKey) ;
    }

    //�E�N���A��ʕ\��
    drawObject(state) ;
    printf("�N���A�ł��B���߂łƂ��������܂��B");
    printf("Q�������Ă��������B");

    //�E�I��
    char input = 0 ;
    while(input != 'q'){
        scanf("%c", &input) ;
    }

    //�E���������
    free(state) ;

    return 0 ;
}


//�E�e��֐���`
//�E�S�I�u�W�F�N�g�ݒ�
void setObject(State* s){

    //�E�O�ǐݒ�
    for(int y = 0 ; y < MAZE_NUM ; ++y){

        for(int x = 0 ; x < MAZE_NUM ; ++x){

            if(x == 0 || x == MAZE_NUM-1 || y == 0 || y == MAZE_NUM -1){
                s->mObjects[x][y] = OBJ_WALL ;
            }
            else{
                s->mObjects[x][y] = OBJ_SPACE ;
            }
        }
    }

    //�E�v���C���[�ݒ�
    s->mPlayerPoint.x = 1 ;
    s->mPlayerPoint.y = 1 ;
    s->mObjects[s->mPlayerPoint.x][s->mPlayerPoint.y] = OBJ_PLAYER ;

    //�E�S�[���ݒ�
    srand((unsigned)time(0UL)) ;    //�E�����̎�ݒ�

    s->mGoalPoint.x = rand()%(MAZE_NUM-3) +1 ;  //�E1�`13�������_���ɐݒ�
    s->mGoalPoint.y = rand()%(MAZE_NUM-3) +1 ;

    if(s->mGoalPoint.x == 1 && s->mGoalPoint.y == 1){   //�E�v���C���[�̈ʒu�Ɠ����������ꍇ
        s->mGoalPoint.x = 13 ;
        s->mGoalPoint.y = 13 ;
    }

    s->mObjects[s->mGoalPoint.x][s->mGoalPoint.y] = OBJ_GOAL ;

    //�E�ז��ȕǐݒ�
    for(int i = 0 ; i < 12 ; ++i){

        Point wallPoint = {rand()%(MAZE_NUM-2) +1,rand()%(MAZE_NUM-2) +1} ;
        Object* changeObj = &(s->mObjects[wallPoint.x][wallPoint.y]) ;
        if(*changeObj == OBJ_SPACE){
            *changeObj = OBJ_WALL ;
        }
    }
    
    //�E�N���A���ʐݒ�
    s->mIsClear = false ;
}

//�E�S�I�u�W�F�N�g�\��
void drawObject(State* s){

    system("cls") ; //�E��ʃN���A

    //�E�ϐ��ݒ�
    int area = 1 ;  //�E���E

    //�E�I�u�W�F�N�g�\��
    if(s->mIsClear == false){   //�E�Q�[��������I���o

        for(int y = 0 ; y < MAZE_NUM ; ++y){

            bool rangeY = false ;   //�E���o�͈͂��ǂ���
            rangeY = (s->mPlayerPoint.y-area <= y && y <= s->mPlayerPoint.y +area) ? true : false ;

            for(int x = 0 ; x < MAZE_NUM ; ++x){

                bool rangeX = false ;   //�E���o�͈͂��ǂ���
                rangeX = (s->mPlayerPoint.x-area <= x && x <= s->mPlayerPoint.x +area) ? true : false ;

                if( rangeX && rangeY ){ //�E���o�͈͓������\��
                    switch(s->mObjects[x][y]){

                        case OBJ_GOAL :
                            printf("G ") ;
                            break ;

                        case OBJ_PLAYER :
                            printf("P ") ;
                            break ;

                        case OBJ_SPACE :
                            printf("  ") ;
                            break ;

                        case OBJ_WALL :
                            printf("��") ;
                            break ;
                    } 
                }
            }

            if( rangeY ){
                printf("\n") ;
            }
        }
    }
    else{   //�E�Q�[���I���S�͈͕\��

        for(int y = 0 ; y < MAZE_NUM ; ++y){

            for(int x = 0 ; x < MAZE_NUM ; ++x){

                switch(s->mObjects[x][y]){

                    case OBJ_GOAL :
                        printf("G ") ;
                        break ;

                    case OBJ_PLAYER :
                        printf("P ") ;
                        break ;

                    case OBJ_SPACE :
                        printf("  ") ;
                        break ;

                    case OBJ_WALL :
                        printf("��") ;
                        break ;
                } 
            }

            printf("\n") ;
        }        
    }
}

//�E���͎擾
char getInput(){

    //�E�ϐ��ݒ�
    char input = 0 ;    //�E�񕶎��ŏ�����

    //�E���͎擾
    while(isalpha(input) == 0){  //�E�����F�A���t�@�x�b�g���󂯎��܂�
        scanf("%c", &input) ;
    }

    //�E���͂��ꂽ�A���t�@�x�b�g��Ԃ�
    return input ;
}

//�E�v���C���[�̈ړ�
void movePlayer(State* s, Point vector){

    //�E�v���C���[�̈ړ�������W�v�Z
    Point nextPoint = {s->mPlayerPoint.x +vector.x, s->mPlayerPoint.y +vector.y} ;

    Object* nextObject = &(s->mObjects[nextPoint.x][nextPoint.y]) ;

    //�E�ړ�
    if(*nextObject == OBJ_SPACE || *nextObject == OBJ_GOAL){

        *nextObject = OBJ_PLAYER ;
        s->mObjects[nextPoint.x-vector.x][nextPoint.y-vector.y] = OBJ_SPACE ;
        s->mPlayerPoint.x = nextPoint.x;
        s->mPlayerPoint.y = nextPoint.y ;
    }
}

//�E�L�����N�^�[�X�V
void update(State* s, char input){

    //�E�v���C���[�̕���
    Point vector ;

    switch(input){

        case 'w' :
            vector.x = 0 ;
            vector.y = -1 ;
            break ;

        case 'z' :
            vector.x = 0 ;
            vector.y = 1 ;
            break ;

        case 'a' :
            vector.x = -1 ;
            vector.y = 0 ;
            break ;

        case 's' :
            vector.x = 1 ;
            vector.y = 0 ;
            break ;

        default :
            vector.x = 0 ;
            vector.y = 0 ;
            break ;
    }

    //�E�v���C���[�ړ�
    movePlayer(s, vector) ;

    //�E�Q�[�����ʐݒ�
    if(s->mPlayerPoint.x == s->mGoalPoint.x && s->mPlayerPoint.y == s->mGoalPoint.y){

        s->mIsClear = true ;
    }
}