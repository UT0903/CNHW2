#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
#define BUFF_SIZE 3000
#define ERR_EXIT(a) { perror(a); exit(1); }

typedef struct{
    int width;
    int height;
}WH;

typedef struct{
    char data[BUFF_SIZE];
    int size;
}DA;
typedef union{
    WH wh; // width and height of the frame
    unsigned char frame[BUFF_SIZE];
    int imgSize;
    char ls_info[BUFF_SIZE]; // ls info
    char file_name[BUFF_SIZE];
    DA data;
}MSG;

typedef enum{
    EXIT,
    WIDTH_HEIGHT,
    IMG_SIZE,
    DATA,
    FRAME,
    LS_INFO,
    LS_REQ,
    UP_REQ,
    DOWN_REQ,
    PLAY_REQ,
    FINISH,
    SERVER,
    NONE,
    ESC_REQ,
    FILE_NOT_FOUND,
}MSG_TYPE;

typedef struct{
    MSG_TYPE type;
    MSG content;
}MESSAGE;


void send_msg(int client_fd, MESSAGE msg){
    if(send(client_fd, &msg, sizeof(MESSAGE), 0) < 0)
        ERR_EXIT("send failed");
}
int recv_msg(int client_fd, MESSAGE *msg){
    if (recv(client_fd, msg, sizeof(MESSAGE), MSG_WAITALL) < 0){
        ERR_EXIT("recv failed");
    }
    return msg->type;
}
int peek_recv_msg(int client_fd, MESSAGE *msg){
    if (recv(client_fd, msg, sizeof(MESSAGE), MSG_PEEK | MSG_DONTWAIT) < 0){
        ERR_EXIT("recv failed");
    }
    return msg->type;
}
MESSAGE new_LS(const char *content){
    MESSAGE msg;
    msg.type = LS_INFO;
    strcpy(msg.content.ls_info, content);
    return msg;
}
MESSAGE new_FINISH(){
    MESSAGE msg;
    msg.type = FINISH;
    return msg;
}
MESSAGE new_WH(int width, int height){
    MESSAGE msg;
    msg.type = WIDTH_HEIGHT;
    msg.content.wh.width = width;
    msg.content.wh.height = height;
    return msg;
}

MESSAGE new_req(MSG_TYPE req_type, const char *more_info){
    MESSAGE msg;
    msg.type = req_type;
    strcpy(msg.content.file_name, more_info);
    return msg;
}

int file_exist (string name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}
std::string newClientDir(){
    int i = 1;
    std::string dir_path;
    while(1){
        dir_path = "client_folder" + to_string(i);
        if(file_exist(dir_path)) i++;
        else break;
    }
    if(mkdir(dir_path.c_str(), S_IRWXU) != 0){
        fprintf(stderr, "Error in newClientDir()\n");
        exit(0);
    }
    return string("./" + dir_path + "/");
}


vector<string> splitStr2Vec(string s, string splitSep){
    vector<string> result;
    int current = 0;
    int next = 0;
    while (next != -1){
        next = s.find_first_of(splitSep, current); 
        if (next != current){
            string tmp = s.substr(current, next - current);
            if(!tmp.empty()){
                result.push_back(tmp);
            }
        }
        current = next + 1;
    }
    return result;
}