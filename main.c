#include "cJSON.h"
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
char token[50];
int initialize(int);
char *send_data(char *);
void print_new();
struct sockaddr_in server;
int m_index = 0;




int initialize(int port){
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2 , 2) , &wsadata);
    if(wsaStartUp != 0){
        printf("ERROR");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}

char *send_data(char *data){
    char *buffer = malloc(1000);
    memset(buffer, 0, 1000);

    int client_socket = socket(AF_INET, SOCK_STREAM , 0);
    if (client_socket == INVALID_SOCKET){
        return buffer;
    }

    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if (can_connect != 0){
        return buffer;
    }

    send(client_socket, data , strlen(data), 0);
    recv(client_socket, buffer, 999, 0);
    closesocket(client_socket);
    return buffer;
}

void print_new(){
    while (1) {
        char *message;
        char data[15];
        sprintf(data, ":%d", m_index);
        message = send_data(data);
        if (message[0] == ':') {
            break;
        }
        printf("%s\n", message);
        m_index++;
    }
}

void fentermenue(int n);
void flogout(int n){    //need change
    char str1[200] = "logout ";
    strcat(str1 , token);
    strcat(str1 , "\n");
    char *response = send_data(str1);
    fentermenue(1);

}
void fmenu(int n);

void fsendtwitter(int n);
void frefresh(int n);
void ftimeline(int n){
    printf("1. send twitter\n");
    printf("2. refresh\n");
    printf("3. back\n");
    printf("entr number of menu you want :\n");
    int a;
    scanf("%d" , &a);
    switch (a) {
        case 1:
            fsendtwitter(1);
            break;
        case 2:
            frefresh(2);
            break;
        case 3:
            fmenu(3);
        default:
            printf("you should enter 1 or 2");
            ftimeline(3);
    }
}
void fmenu(int n);
void fsendtwitter(int n){
    printf("enter your twitt :\n");
    char tweet[100];
    char str1[200] = "send tweet ";
    char str2[] = ", ";
    scanf("%s" , tweet);
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , tweet);
    strcat(str1 , "\n");
    strcat(str1 , "\0");
    printf("%s" , str1);
    char *response = send_data(str1);

    printf("your tweet was send !\n");

    fmenu(1);
}
void fmenu(int n);
void flike(int n){
    char str1[200] = "like ";
    char str2[] = ", ";
   // char str3[] = "\n";
    char tweetid[20];
    printf("enter your tweet id :\n");
    scanf("%s" , tweetid);
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , tweetid);
    strcat(str1 , "\n");
    char *response = send_data(str1);
    printf("you have liked this comment !\n");

    fmenu(1);
}
void fmenu(int n);
void fcomment(int n){
    char str1[200] = "comment ";
    char str2[] = ", ";
    char str3[] = "\n";
    char tweet_id[20];
    char comment[20];
    printf("enter tweet id :\n");
    scanf("%s" , tweet_id);
    printf("enter your comment :\n");
    scanf("%s" , comment);
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , tweet_id);
    strcat(str1 , str2);
    strcat(str1 , comment);
    strcat(str1 , "\n");
    char *response = send_data(str1);
    printf("your comment was send !\n");
    fmenu(1);
}

void fseecomments(int n) {
    int id_check = 0;
    printf("enter the id you want to see comments :");
    scanf("%d" , &id_check);

    char str1[200] = "refresh ";
    char str2[] = "\n";
    strcat(str1, token);
    strcat(str1, str2);
    char *server_reply = send_data(str1);

    //   while(1==1){
    cJSON *root = cJSON_Parse(server_reply);
    cJSON *id = cJSON_GetObjectItem(root, "id");
    cJSON *type = cJSON_GetObjectItem(root, "type");
    cJSON *message = cJSON_GetObjectItem(root, "message");
    cJSON *tweet = NULL;
    cJSON_ArrayForEach(tweet, message) {
        //    int b;
        //b = strcmp(id->valuestring , "");
        //    if(b == 0){
        //      break;
        //}
        //printf("%s" , type->valuestring);
        cJSON *id = cJSON_GetObjectItem(root, "id");
        cJSON *author = cJSON_GetObjectItem(root, "author");
        cJSON *content = cJSON_GetObjectItem(root, "content");
        cJSON *comments = cJSON_GetObjectItem(root, "comments");
        cJSON *likes = cJSON_GetObjectItem(root, "likes");
        if(id->valueint == id_check){
            printf("%s" , comments->valuestring);
        }

    }
}
void frefresh(int n){
    char str1[200]="refresh ";
    char str2[]= "\n";
    strcat(str1 , token);
    strcat(str1 , str2);
    char *server_reply = send_data(str1);
         //   while(1==1){
            cJSON *root = cJSON_Parse(server_reply);
           // cJSON *id = cJSON_GetObjectItem(root , "id");
            cJSON *type = cJSON_GetObjectItem(root , "type");
            cJSON *message = cJSON_GetObjectItem(root , "message");
            //cJSON *tweet = NULL;
            int message_size = cJSON_GetArraySize(message);
            for(int i=0 ; i< message_size ; i++) {
                //cJSON_ArrayForEach(tweet , message){
                //    int b;
                //b = strcmp(id->valuestring , "");
                //    if(b == 0){
                //      break;
                //}
                //printf("%s" , type->valuestring);
                cJSON *elem = cJSON_GetArrayItem(message , i);
                cJSON *id = cJSON_GetObjectItem(elem, "id");
                cJSON *author = cJSON_GetObjectItem(elem, "author");
                cJSON *content = cJSON_GetObjectItem(elem, "content");
                cJSON *comments = cJSON_GetObjectItem(elem, "comments");
                cJSON *likes = cJSON_GetObjectItem(elem, "likes");

                printf("id :%d\n", id->valueint);

                printf("author :%s\n", author->valuestring);

                printf("content :%s\n", content->valuestring);

                printf("comments :%d\n", comments->valueint);

                printf("likes :%d\n", likes->valueint);

            }
       //}



    printf("enter the number you wt :\n");
    printf("1. like\n2. comment\n3. see comments\n4. back\n");
    int a;
    scanf("%d" , &a);
    switch (a) {
        case 1:
            flike(1);
            break;
        case 2:
            fcomment(2);
            break;
        case 3:
            fseecomments(3);
            break;

        case 4:
            ftimeline(4);
            break;
    }
}

int funfollow(int n){
    char string[20];
    printf("enter the name to unfollow :\n");
    scanf("%s" , string);
    char str1[200] = "unfollow ";
    char str2[] = ", ";
    char str3[] = "\n";
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , string);
    strcat(str1 , str3);
    char *message = send_data(str1);
    fmenu(1);
}
void fmenu(int n);
void ffollow(int n){
    char string[20];
    printf("enter the name to follow :\n");
    scanf("%s" , string);
    char str1[200] = "follow ";
    char str2[] = ", ";
    char str3[] = "\n";
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , string);
    strcat(str1 , str3);
    char *message = send_data(str1);
    fmenu(1);
}
void fsearch(int n) {
    printf("enter the id you are looking for :\n");
    char name[20];
    char str1[200] = "search ";
    char str2[] = ", ";
    char str3[] = "\n";
    scanf("%s", name);
    strcat(str1, token);
    strcat(str1, str2);
    strcat(str1, name);
    strcat(str1, str3);
    char *server_reply = send_data(str1);
    cJSON *root = cJSON_Parse(server_reply);
    cJSON *type = cJSON_GetObjectItem(root, "type");
    if (strcmp(type->valuestring, "Error") == 0) {
        printf("an error has happend\n");
        fsearch(1);
    }
        cJSON *message = cJSON_GetObjectItem(root, "message");

        cJSON *username = cJSON_GetObjectItem(message, "username");
        cJSON *bio = cJSON_GetObjectItem(message, "bio");
        cJSON *numberOfFollowers = cJSON_GetObjectItem(message, "numberOfFollowers");
        cJSON *numberOfFollowings = cJSON_GetObjectItem(message, "numberOfFollowings");
        cJSON *followStatus = cJSON_GetObjectItem(message, "followStatus");
        cJSON *allTweets = cJSON_GetObjectItem(message, "allTweets");

        printf("user name :\n%s\n", username->valuestring);
        if (strcmp(bio->valuestring, "") == 0) {
            printf(" bio : there is nothing left to say now...!\n");
        } else {
            printf("bio :\n%s\n", bio->valuestring);
        }
        printf("numberOfFollowers :\n%d\n", numberOfFollowers->valueint);
        printf("numberOfFollowings :\n%d\n", numberOfFollowings->valueint);
        printf("followStatus :\n%s\n", followStatus->valuestring);
        //  printf("allTweets :\n%s\n" , allTweets->valuestring);

        int i;
        for (i = 0; i < cJSON_GetArraySize(allTweets); i++) {
            cJSON *subitem = cJSON_GetArrayItem(allTweets, i);
            cJSON *id = cJSON_GetObjectItem(subitem, "id");
            cJSON *author = cJSON_GetObjectItem(subitem, "author");
            cJSON *content = cJSON_GetObjectItem(subitem, "content");
            cJSON *comments = cJSON_GetObjectItem(subitem, "comments");

            cJSON *likes = cJSON_GetObjectItem(subitem, "likes");
            printf("id :%d\n", id->valueint);
            printf("author :%s\n", author->valuestring);
            printf("contents :%s\n", content->valuestring);
            printf("likes :%d\n", likes->valueint);
            printf("number of comments : %d\n", comments->valueint);
        }
        int b;
        b = strcmp(followStatus->valuestring, "Followed");
        if (b == 0) {
            // int b;
            printf("enter the number you want :\n");
            printf("1. unfollow\n");
            printf("2. back\n");
            int a = 1;
            scanf("%d", &a);
            switch (a) {
                case 1:
                    funfollow(1);
                    break;
                case 2:
                    fmenu(2);
                    break;
            }

        } else {

            printf("enter the number you want :\n");
            printf("1. follow\n");
            printf("2. back\n");
            int a;
            scanf("%d", &a);
            switch (a) {
                case 1:
                    ffollow(1);
                    break;
                case 2:
                    fmenu(2);
                    break;
            }

        }

    }

void ftweeterprofile(int n){
    char str1[200] = "profile ";
    strcat(str1 , token);
    strcat(str1 , "\n");
    char *server_reply = send_data(str1);
    printf("1\n");
    cJSON *root = cJSON_Parse(server_reply);
    cJSON *message = cJSON_GetObjectItem(root , "message");

    printf("2\n");
    cJSON *username = cJSON_GetObjectItem(message , "username");
    cJSON *bio = cJSON_GetObjectItem(message , "bio");
    cJSON *numberOfFollowers = cJSON_GetObjectItem(message , "numberOfFollowers");
    cJSON *numberOfFollowings = cJSON_GetObjectItem(message , "numberOfFollowings");
    cJSON *followStatus = cJSON_GetObjectItem(message , "followStatus");
    cJSON *allTweets = cJSON_GetObjectItem(message , "allTweets");
//printf("3\n");
printf("following status :%s\n" , followStatus->valuestring);
    printf("user name :%s\n" , username->valuestring);
  //  printf("4\n");
    if(strcmp(bio->valuestring , "") == 0){
        printf("there is nothing left to say now...!\n");
    }
    else {
        printf("bio :%s\n", bio->valuestring);
    }
    printf("numberOfFollowers :%d\n" , numberOfFollowers->valueint);
    printf("numberOfFollowings :%d\n" ,numberOfFollowings->valueint );
    int i;
    for (i = 0 ; i < cJSON_GetArraySize(allTweets) ; i++){
        cJSON * subitem = cJSON_GetArrayItem(allTweets, i);
        cJSON *  id = cJSON_GetObjectItem(subitem, "id");
        cJSON *author = cJSON_GetObjectItem(subitem, "author");
        cJSON * contents = cJSON_GetObjectItem(subitem, "content");
        cJSON * comments = cJSON_GetObjectItem(subitem, "comments");

        cJSON * likes = cJSON_GetObjectItem(subitem, "likes");
        printf("id :%d\n" , id->valueint);
        printf("author :%s\n" , author->valuestring);
        printf("hello\n");
        printf("contents :%s\n" , contents->valuestring);

        printf("likes :%d\n" , likes->valueint);
    }


        int a=1;
    printf("1. back\n");
    scanf("%d" , &a);
    fmenu(a);

}
void fmenu(int n);
void fsetbio(int n){
    char str1[200] = "profile ";
    strcat(str1 , token);
    strcat(str1 , "\n");
    char *server_reply = send_data(str1);
    printf("1\n");
    cJSON *root = cJSON_Parse(server_reply);
    cJSON *message = cJSON_GetObjectItem(root , "message");
    cJSON *bio_check = cJSON_GetObjectItem(message , "bio");
    if(strcmp(bio_check->valuestring , "") != 0){
        printf("Bio is already updated.");
        printf("enter any number to back:\n");
        int a;
        scanf("%d" , &a);
        fmenu(a);
    }
    else {
        char bio[50];
        char str1[200] = "set bio ";
        char str2[] = ", ";
        printf("enter your bio :\n");
        scanf("%s", bio);
        strcat(str1, token);
        //  printf("%s\n" , str1);
        strcat(str1, str2);
//    printf("%s\n" , str1);

        strcat(str1, bio);

        strcat(str1, "\n");
        //printf("%s" , str1);
        char *message = send_data(str1);
        printf("enter any number to back:\n");
        int a;
        scanf("%d", &a);
        fmenu(a);
    }
}
void fmenu(int n);
void fchangepassword(int n){
    char curent_pass[20];
    char new_pass[20];
    printf("enter your curent password :\n");
    scanf("%s" , curent_pass);
    printf("enter your new password :\n");
    scanf("%s" , new_pass);
    char str1[200] = "change password ";
    char str2[] = ", ";
    strcat(str1 , token);
    strcat(str1 , str2);
    strcat(str1 , curent_pass);
    strcat(str1 , str2);
    strcat(str1 , new_pass);
    strcat(str1 , "\n");
    char *message = send_data(str1);
    printf("%s\n" , message);

    printf("enter any number to back:\n");
    int a;
    scanf("%d" , &a);
    fmenu(a);
     }
void fpersonalarea(int n){
    printf("enter number of menu you want :\n");
    printf("1. set bio\n");
    printf("2. change yor password\n");
    int a;
    scanf("%d" , &a);
    switch (a) {
        case 1:
            fsetbio(1);
            break;
        case 2:
            fchangepassword(2);
            break;
        default:
            printf("you should enter 1 ot 2\n");
            fpersonalarea(3);
    }
}

void fmenu(int n){
    printf("1. timeline\n");
    printf("2. search\n");
    printf("3. twitter profile\n");
    printf("4. personal area\n");
    printf("5. logout\n");
    printf("enter number of menu you want :\n");
    int a;
    scanf("%d" , &a);
    switch (a) {
        case 1:
            ftimeline(1);
            break;
        case 2:
            fsearch(2);
            break;
        case 3:
            ftweeterprofile(3);
            break;
        case 4:
            fpersonalarea(4);
            break;
        case 5:
            flogout(5);
            break;
        default:
            printf("you should enter a number between 1 to 5\n");
            fmenu(6);
    }
}
void fentermenue(int n);
void fsignup(int n){ //need change base on server
    char name[20];
    char pass[20];
    char str1[200];
    strcpy(str1 , "signup ");
    char str2[]=", ";
    printf("enter your name :\n");
    scanf("%s" , name);
    printf("enter your password :\n");
    scanf("%s" , pass);
    strcat(str1 , name);
    strcat(str1 , str2);
    strcat(str1 , pass);
    strcat(str1 , "\n");
    strcat(str1 , "\0");
    char *message = send_data(str1);
    printf("%s" , message);


        fentermenue(1);
    //}
}
void fmenu(int n);
void flogin(int n){  //need change base on server
    char name[10];
    char pass[10];
    char str1[200];
    strcpy(str1 , "login ");
    char str2[]=", ";
    printf("enter your name :\n");
    scanf("%s" , name);
    printf("enter your pass : \n");
    scanf("%s" , pass);
    strcat(str1 , name);
    strcat(str1 , str2);
    strcat(str1 , pass);
    strcat(str1 , "\n");
    strcat(str1 , "\0");

    char *message = send_data(str1);
    printf("%s\n" , message);
    cJSON *root = cJSON_Parse(message);
    cJSON *string = cJSON_GetObjectItem(root , "message");
    strcpy(token , string->valuestring);
    fmenu(1);
}
void fentermenue(int n){
    printf("welcome!!\n");

    int a;
    printf("1.signup\n2.login\n");
    scanf("%d" , &a);
    if(a==1){
        fsignup(1);
    }
    if(a==2){
        flogin(2);
    }
}
int main() {
    int a;
    a = initialize(12345);
    if(a == 0 ){
        int n = 1;
        fentermenue(n);
    }

    return 0;
}
