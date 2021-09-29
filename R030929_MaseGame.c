#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>  //・アルファベットかどうか調べる関数 isalpha()を使用するため

#define MAZE_NUM 15 //・迷路の範囲

//・各キャラクター（オブジェクト）設定
typedef enum{
    OBJ_WALL,   //・壁
    OBJ_SPACE,  //・道
    OBJ_PLAYER, //・プレイヤー
    OBJ_GOAL,   //・ゴール
}Object ;

//・座標構造体
typedef struct _Point{
    int x ;
    int y ;
}Point ;

//・全オブジェクトの状態を表す構造体
typedef struct _State{
    Object mObjects[MAZE_NUM][MAZE_NUM] ;   //・オブジェクトの配置
    Point mPlayerPoint ;    //・プレイヤーの場所
    Point mGoalPoint ;      //・ゴールの場所
    bool mIsClear ;         //・クリア結果
}State ;

//・各種関数宣言
void setObject(State* s);   //・全オブジェクト設定
void drawObject(State* s);  //・全オブジェクト表示
char getInput() ;           //・入力取得
void movePlayer(State* s, Point vector);//・プレイヤーの移動
void update(State* s, char input);      //・オブジェクト更新


/*****************************
メイン関数です
******************************/
int main(void){

    //・構造体作成
    State* state = (State*) malloc(sizeof(State)) ;

    //・設定
    setObject(state) ;

    //・メインループ
    while(state->mIsClear == false){

        //・描画
        drawObject(state) ;
        printf("ゴールGを目指してください。") ;
        printf("操作方法 w:↑ z:↓ a:← s:→ \n") ;
        printf("input key: ") ;

        //・入力取得
        char inputKey ;
        inputKey = getInput() ;

        //・更新
        update(state, inputKey) ;
    }

    //・クリア画面表示
    drawObject(state) ;
    printf("クリアです。おめでとうございます。");
    printf("Qを押してください。");

    //・終了
    char input = 0 ;
    while(input != 'q'){
        scanf("%c", &input) ;
    }

    //・メモリ解放
    free(state) ;

    return 0 ;
}


//・各種関数定義
//・全オブジェクト設定
void setObject(State* s){

    //・外壁設定
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

    //・プレイヤー設定
    s->mPlayerPoint.x = 1 ;
    s->mPlayerPoint.y = 1 ;
    s->mObjects[s->mPlayerPoint.x][s->mPlayerPoint.y] = OBJ_PLAYER ;

    //・ゴール設定
    srand((unsigned)time(0UL)) ;    //・乱数の種設定

    s->mGoalPoint.x = rand()%(MAZE_NUM-3) +1 ;  //・1～13をランダムに設定
    s->mGoalPoint.y = rand()%(MAZE_NUM-3) +1 ;

    if(s->mGoalPoint.x == 1 && s->mGoalPoint.y == 1){   //・プレイヤーの位置と同じだった場合
        s->mGoalPoint.x = 13 ;
        s->mGoalPoint.y = 13 ;
    }

    s->mObjects[s->mGoalPoint.x][s->mGoalPoint.y] = OBJ_GOAL ;

    //・邪魔な壁設定
    for(int i = 0 ; i < 12 ; ++i){

        Point wallPoint = {rand()%(MAZE_NUM-2) +1,rand()%(MAZE_NUM-2) +1} ;
        Object* changeObj = &(s->mObjects[wallPoint.x][wallPoint.y]) ;
        if(*changeObj == OBJ_SPACE){
            *changeObj = OBJ_WALL ;
        }
    }
    
    //・クリア結果設定
    s->mIsClear = false ;
}

//・全オブジェクト表示
void drawObject(State* s){

    system("cls") ; //・画面クリア

    //・変数設定
    int area = 1 ;  //・視界

    //・オブジェクト表示
    if(s->mIsClear == false){   //・ゲーム中限定的視覚

        for(int y = 0 ; y < MAZE_NUM ; ++y){

            bool rangeY = false ;   //・視覚範囲かどうか
            rangeY = (s->mPlayerPoint.y-area <= y && y <= s->mPlayerPoint.y +area) ? true : false ;

            for(int x = 0 ; x < MAZE_NUM ; ++x){

                bool rangeX = false ;   //・視覚範囲かどうか
                rangeX = (s->mPlayerPoint.x-area <= x && x <= s->mPlayerPoint.x +area) ? true : false ;

                if( rangeX && rangeY ){ //・視覚範囲内だけ表示
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
                            printf("□") ;
                            break ;
                    } 
                }
            }

            if( rangeY ){
                printf("\n") ;
            }
        }
    }
    else{   //・ゲーム終了全範囲表示

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
                        printf("□") ;
                        break ;
                } 
            }

            printf("\n") ;
        }        
    }
}

//・入力取得
char getInput(){

    //・変数設定
    char input = 0 ;    //・非文字で初期化

    //・入力取得
    while(isalpha(input) == 0){  //・条件：アルファベットを受け取るまで
        scanf("%c", &input) ;
    }

    //・入力されたアルファベットを返す
    return input ;
}

//・プレイヤーの移動
void movePlayer(State* s, Point vector){

    //・プレイヤーの移動する座標計算
    Point nextPoint = {s->mPlayerPoint.x +vector.x, s->mPlayerPoint.y +vector.y} ;

    Object* nextObject = &(s->mObjects[nextPoint.x][nextPoint.y]) ;

    //・移動
    if(*nextObject == OBJ_SPACE || *nextObject == OBJ_GOAL){

        *nextObject = OBJ_PLAYER ;
        s->mObjects[nextPoint.x-vector.x][nextPoint.y-vector.y] = OBJ_SPACE ;
        s->mPlayerPoint.x = nextPoint.x;
        s->mPlayerPoint.y = nextPoint.y ;
    }
}

//・キャラクター更新
void update(State* s, char input){

    //・プレイヤーの方向
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

    //・プレイヤー移動
    movePlayer(s, vector) ;

    //・ゲーム結果設定
    if(s->mPlayerPoint.x == s->mGoalPoint.x && s->mPlayerPoint.y == s->mGoalPoint.y){

        s->mIsClear = true ;
    }
}