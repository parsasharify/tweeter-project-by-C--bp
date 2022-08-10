#include <stdio.h>
#include <winsock2.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>

typedef struct string string;

typedef struct dynamic dynamic;

void like(char buffer[1000]);
char online_username[100][20];
char token[100][32];
int  seen_tweet_ids[100][100];
int number_of_online = 0;
void signup(char buffer[1000] , int client_socket);
void login(char buffer[1000] , int client_socket);
void send_tweet(char buffer[1000]);
void comments(char buffer[1000]);
void search(char buffer[1000] , int client_socket);
void setbio(char buffer[1000] , int client_socket);
void logout(char buffer[1000]);
void follow(char  buffer[1000]);
void profile(char buffer[1000] , int client_socket);
void unfollow(char buffer[1000]);
void refresh(char buffer[1000] , int client_socket);
void deletetweet(char buffer[1000]);
void changepassword(char buffer[1000]);
void searchtweet(char buffer[1000] , int client_socket);
void seecomments(char buffer[1000] , int client_socket);

int initialize(int port) {
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2 , 2) , &wsadata);
    if(wsa_res !=0){
        printf("Faild to run wsadata");
        return -1;//???????????????????
    }

    int server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if(server_socket == INVALID_SOCKET) {
        printf("Could not create socket");
        return INVALID_SOCKET;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int bind_res = bind(server_socket , (struct sockaddr*)&server_addr, sizeof (server_addr));
    if(bind_res == SOCKET_ERROR){
        printf("Bind failed");
        return INVALID_SOCKET;
    }
    listen(server_socket , SOMAXCONN);

    return server_socket;
}

void start(int server_socket) {
    int len = sizeof (struct sockaddr_in);

    int client_socket;
    struct sockaddr_in client_addr;

    char *messages[1000];
    int size = 0;

    while((client_socket = accept(server_socket , (struct sockaddr*)& client_addr , &len)) != INVALID_SOCKET){
        printf("Client connected");
        char buffer[1000];
        char type_of_message[20];
        memset(buffer , 0 , 1000);
        printf("1\n");
        recv(client_socket , buffer , 999 , 0);
        printf("2\n");
        sscanf(buffer , "%s" , type_of_message);
        printf("3\n");
        printf("%s" , type_of_message);
        if(strcmp(type_of_message , "signup") == 0){
            printf("1\n");
            signup(buffer , client_socket);
        }

        else if(strcmp(type_of_message , "login") == 0){
            login(buffer , client_socket);
        }
        else if(strcmp(type_of_message , "send") == 0){
            printf("0\n");
            send_tweet(buffer);
        }
        else if(strcmp(type_of_message , "like") == 0){
            like(buffer);
        }
        else if(strcmp(type_of_message , "comment") == 0){
            comments(buffer);
        }
        else if(strcmp(type_of_message , "search") == 0){
            search( buffer ,  client_socket);
        }
        else if(strcmp(type_of_message, "set") == 0){
            printf("11\n");
            setbio( buffer ,  client_socket);
        }
        else if(strcmp(type_of_message , "logout") == 0){
            logout(buffer);
        }
        else if(strcmp(type_of_message , "follow") == 0){
            follow(buffer);
        }
        else if(strcmp(type_of_message , "profile") == 0){
            profile( buffer,  client_socket);
        }
        else if(strcmp(type_of_message , "unfollow") == 0){
            unfollow(buffer);
        }
        else if(strcmp(type_of_message , "refresh") == 0){
            refresh( buffer ,  client_socket);
        }
        else if(strcmp(type_of_message , "deletetweet") == 0){
            printf("23\n");
            deletetweet( buffer);
        }
        else if(strcmp(type_of_message , "changepassword") == 0){
            changepassword(buffer);
        }
        else if(strcmp(type_of_message , "searchtweet") == 0){
            searchtweet(buffer ,  client_socket);
        }
        else if(strcmp(type_of_message , "seecomments") == 0){
             seecomments( buffer ,  client_socket);
        }
        closesocket(client_socket);
    }
}
int main() {
    int server_socket = initialize(12345);
    start( server_socket);
    return 0;
}
void signup(char buffer[1000] , int client_socket){
    char userneme[20];
    char password[20];
    char response[50];
    char car;
    sscanf(buffer , "signup %s %s" , userneme , password);
    userneme[strlen(userneme)-1] = '\0';
    FILE *filePointer ;
    char str[50];
    strcpy(str , userneme);
    strcat(str , ".user.json");
    filePointer = fopen(str, "r") ;

    if ( filePointer != NULL ){

        printf("this username is already taken.");

    }

    else {
        filePointer = fopen(str , "w");
        cJSON * root = NULL;
        root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "username", userneme);
        cJSON_AddStringToObject(root, "password", password);
        cJSON_AddStringToObject(root, "bio", "");
        cJSON * followers = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "followers",  followers);
        cJSON * followings = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "followings",  followings);
        cJSON * personalTweets = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "personaltweets",  personalTweets);
        cJSON * tweet_id_seen_refresh = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "tweet_id_seen_refresh",  tweet_id_seen_refresh);
        cJSON * liked_tweets = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "liked_tweets",  liked_tweets);


        fprintf(filePointer, "%s" , cJSON_PrintUnformatted(root));
        fclose(filePointer);
        printf("usersigned up\n");

    }
}

void login(char buffer[1000] , int client_socket){
    char username[20];
    char password[20];
    char dataToBeRead[1000];
    sscanf(buffer , "login %s %s" , username , password);
    username[strlen(username)-1] = '\0';
    FILE *filePointer ;
    char str[50];
    strcpy(str , username);
    strcat(str , ".user.json");
    filePointer = fopen(str, "r") ;
    if ( filePointer == NULL )
    {
        printf("wrong user name\n");
    }
    else {
        fgets ( dataToBeRead, 1000 , filePointer );
        cJSON *root = cJSON_Parse(dataToBeRead);
        cJSON *pass = cJSON_GetObjectItem(root , "password");
        char *string = cJSON_PrintUnformatted(pass);

        for(int i=0 ; i<=strlen(string) ; i++){
            string[i] = string[i+1];
        }
        string[strlen(string)-1] = '\0';
        //printf("%s" , string);
        if(strcmp(string, password) == 0 ){
            printf("loged in\n");
            //give token
            strcpy(online_username[number_of_online] , username);
            int len = 32;
            static const char alphanum[] =     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            srand(number_of_online);
            for (int i = 0; i < len; ++i) {
                token[number_of_online][i] = alphanum[rand() % 52 ];
            }
            //  printf("%s" , token[number_of_online]);
            for(int x = 0 ; x<100 ; x++){
                seen_tweet_ids[number_of_online][x] = -1;

            }
            cJSON * response = NULL;
            response = cJSON_CreateObject();
            cJSON_AddStringToObject(response, "type",  "Token");
            // cJSON *json_token = cJSON_Parse(token[number_of_online]);

            cJSON_AddStringToObject(response, "message", token[number_of_online]);
            number_of_online++;
            char *string1 = cJSON_PrintUnformatted(response);
            // printf("%s" , string1);
            send(client_socket , string1 , strlen(string1) , 0);
            printf("user loged in\n");
        }
        else{
            printf("wrong password\n");
        }
    }

}

void send_tweet(char buffer[1000]){//check token change---------
    char token_of_message[32];
    char tweet[50];
    int i , j;
    sscanf(buffer , "send tweet %s %s" , token_of_message , tweet);
    token_of_message[strlen(token_of_message)-1] = '\0';

    //search if token is valied?
    for( i=0 ; i<100; i++){
        if(strcmp(token[i] , token_of_message) == 0){
            break;
        }
    }
    if(i>100){
        printf("bad format request\n");
    }
    //search file to find username
    char filename[50];
    strcpy(filename , online_username[i]);
    strcat(filename , ".user.json");

    //find tweet id with for
    FILE *filePointer ;
    char tweet_file_name[50];
    char number[2];
    for(j=0 ; ;j++){
        sprintf(number , "%d" , j);
        strcpy(tweet_file_name , number);
        strcat(tweet_file_name , ".tweet.json");
        filePointer = fopen(tweet_file_name, "r");
        if(filePointer == NULL){
            fclose(filePointer);
            break;
        }
        fclose(filePointer);
    }
    printf("3\n");
    cJSON * tweet_info = NULL;
    tweet_info = cJSON_CreateObject();
    cJSON * comment = NULL;
    comment = cJSON_CreateObject();
    cJSON_AddNumberToObject(tweet_info, "id", j);
    cJSON_AddStringToObject(tweet_info, "author", online_username[i]);
    cJSON_AddStringToObject(tweet_info, "content", tweet);
    cJSON_AddItemToObject(tweet_info, "comments", comment);
    cJSON_AddNumberToObject(tweet_info, "likes", 0);
    cJSON_AddNumberToObject(tweet_info, "is_deleted", 0);

    char *tweet_into_string = cJSON_PrintUnformatted(tweet_info);
    filePointer = fopen(tweet_file_name , "w");
    fprintf(filePointer, "%s" , tweet_into_string);
    fclose(filePointer);
    filePointer = fopen(filename , "r");
    char dataToBeRead[1000];
    fgets ( dataToBeRead, 1000, filePointer );
    fclose(filePointer);
    cJSON *user_info = cJSON_Parse(dataToBeRead);
    cJSON *personalTweets = cJSON_GetObjectItem(user_info, "personalTweets");
    cJSON_AddItemToArray(personalTweets , tweet_info);
    //add one tweet to personal tweet info
    cJSON_ReplaceItemInObjectCaseSensitive(user_info, "personalTweets", personalTweets);
    //save new info
    fopen(filename , "w");
    fprintf(filePointer, "%s" , cJSON_PrintUnformatted(user_info));
    fclose(filePointer);
    printf("tweet was send");

}


void like(char buffer[1000]){
    //search if token is valied?
    printf("11\n");
    char token_of_message[32];
    int id_of_tweet_to_like;
    int i , flag=0;
    sscanf(buffer , "like %s %d" , token_of_message , &id_of_tweet_to_like);
    token_of_message[strlen(token_of_message)-1] = '\0';
    for(i=0 ; i<100 ; i++){
        if(strcmp(token[i] , token_of_message) == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 1) {
        printf("22\n");
        char user_file_name[50];
        strcpy(user_file_name, online_username[i]);
        strcat(user_file_name, ".user.json");
        FILE *filePointer;
        char str[500];
        char dataToBeRead[1000];
        int flag3 = 0;
        filePointer = fopen(user_file_name, "r");
        //catch string-------------------------------------
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root1 = cJSON_Parse(dataToBeRead);
        cJSON *liked_tweets = cJSON_GetObjectItem(root1, "liked_tweets");
        for (int x = 0; x < cJSON_GetArraySize(liked_tweets); x++) {
            cJSON *liked_id = cJSON_GetArrayItem(liked_tweets, x);
            if (liked_id->valueint == id_of_tweet_to_like) {
                flag3 = 1;
                break;
            }
        }
        if (flag3 == 1) {
            printf("you have liked this tweet before\n");
        }
        if (flag3 == 0) {
            cJSON * pJsonRoot = NULL;
            pJsonRoot = cJSON_CreateObject();
            cJSON_SetIntValue(pJsonRoot, id_of_tweet_to_like);
            cJSON_AddItemToArray(liked_tweets , pJsonRoot);
            filePointer = fopen(user_file_name, "w");
            //catch string-------------------------------------
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root1));
            fclose(filePointer);
            //cJSON_AddNumberToObject(liked_tweets , id_of_tweet_to_like);
            char tweet_file_name[20];
            sprintf(tweet_file_name, "%d.tweet.json", id_of_tweet_to_like);
            //getting tweet id

            filePointer = fopen(tweet_file_name, "r");
            //catch string-------------------------------------
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            printf("%s\n", dataToBeRead);
            printf("33\n");
            cJSON *root = cJSON_Parse(dataToBeRead);
            printf("uuuuu\n");
            cJSON *likes = cJSON_GetObjectItem(root, "likes");
            printf("wwwww\n");
            printf("%d\n", likes->valueint);
            printf("rrr\n");
            //likes->valueint+=1;
            //   printf("%d\n" , likes->valueint+=1);
            cJSON_SetIntValue(cJSON_GetObjectItem(root, "likes"), likes->valueint += 1);
            printf("--------------\n");
            // cJSON_DeleteItemFromObjectCaseSensitive(root, "like");
            //cJSON_AddNumberToObject(root, "like", like->valueint);

            cJSON *author_of_tweet = cJSON_GetObjectItem(root, "author");
            filePointer = fopen(tweet_file_name, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root));
            printf("%s", cJSON_PrintUnformatted(root));
            fclose(filePointer);
            //  char *author_of_tweet_string = cJSON_Print(author_of_tweet);
//printf("--------%s\n\n" , author_of_tweet_string);
            char writer_file_name[50];

            strcpy(writer_file_name, author_of_tweet->valuestring);//has problem------------------------------

            strcat(writer_file_name, ".user.json");///////////

            filePointer = fopen(writer_file_name, "r");

            fgets(dataToBeRead, 1000, filePointer);

            fclose(filePointer);

            cJSON *writer_info = cJSON_Parse(dataToBeRead);
            cJSON *personalTweets = cJSON_GetObjectItem(writer_info, "personalTweets");
            int array_of_tweets_size = cJSON_GetArraySize(personalTweets);
            for (i = 0; i < array_of_tweets_size; i++) {
                cJSON *elem = cJSON_GetArrayItem(personalTweets, i);
                cJSON *id = cJSON_GetObjectItem(elem, "id");
                if (id->valueint == id_of_tweet_to_like) {
                    break;
                }
            }
            //cJSON_ReplaceItemInArray(personalTweets, i, root);//?????????????????????
            // cJSON_ReplaceItemInObject(writer_info, "personalTweets", personalTweets);
            cJSON_DeleteItemFromArray(personalTweets, i);
            cJSON_AddItemToArray(personalTweets, root);
            filePointer = fopen(writer_file_name, "w");////////////
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(writer_info));
            fclose(filePointer);
            printf("you liked id you wanted");
        }
    }
    else{
        printf("token doesnt exist\n");
    }
}



void comments(char buffer[1000]){
    char token_of_message[32];
    int tweet_id_to_comment;
    char comment_message[50];
    int i , flag = 0;
    sscanf(buffer , "comment %s%d, %s" , token_of_message , &tweet_id_to_comment , comment_message);
    //search if token is valied
    token_of_message[strlen(token_of_message)-1] = '\0';

    //search if token is valied?
    for(i=0 ; i<100 ; i++){
        if(strcmp(token[i] , token_of_message) == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        printf("invalid token\n");
    }

    if(flag == 1) {
        char tweet_file_name[50];
        char dataToBeRead[1000];
        sprintf(tweet_file_name , "%d" , tweet_id_to_comment);
        strcat(tweet_file_name , ".tweet.json");
        FILE *filePointer;
        filePointer = fopen(tweet_file_name , "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON* root = cJSON_Parse(dataToBeRead);
        cJSON*comments = cJSON_GetObjectItem(root , "comments");
        cJSON*author = cJSON_GetObjectItem(root , "author");
        cJSON_AddStringToObject(comments , online_username[i], comment_message);
        filePointer = fopen(tweet_file_name , "w");
        fprintf(filePointer, "%s", cJSON_PrintUnformatted(root));
        fclose(filePointer);//tweet
        char writer_file_name[50];
        strcpy(writer_file_name , author->valuestring);
        strcat(writer_file_name , ".user.json");
        filePointer = fopen(writer_file_name , "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        printf("www\n");
        cJSON* root2 = cJSON_Parse(dataToBeRead);
        cJSON*personalTweets = cJSON_GetObjectItem(root , "personalTweets");
        for(i=0 ; i< cJSON_GetArraySize(personalTweets) ; i++){

            cJSON *elem = cJSON_GetArrayItem(personalTweets, i);
            cJSON *id = cJSON_GetObjectItem(elem, "id");
            if (id->valueint == tweet_id_to_comment) {
                printf("aaa\n");
                break;
            }
        }

        cJSON_DeleteItemFromArray(personalTweets , i);
        cJSON_AddItemToArray(personalTweets , root);
        //cJSON_DeleteItemFromObject(root2 , "personalTweets");
        cJSON_ReplaceItemInObject(root2 , "personalTweets" , personalTweets);
        //cJSON_AddItemToObject(root2 , "personalTweets" , personalTweets);

        printf("%s" , cJSON_PrintUnformatted(root2));
        filePointer = fopen(writer_file_name, "w");
        fprintf(filePointer, "%s", cJSON_PrintUnformatted(root2));
        fclose(filePointer);
        printf("your comment was send\n");
        //leave message
    }
}

void search(char buffer[1000] , int client_socket){
    char token_of_message[32];
    char user_to_search[20];
    int i , flag =0;
    sscanf(buffer , "search %s %s" , token_of_message , user_to_search);
    token_of_message[strlen(token_of_message)-1] = '\0';
    printf("0\n");
    //search if token is valied?
    for(i=0 ; i<100 ; i++){
        if(strcmp(token[i] , token_of_message) == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        printf("invalid token\n");
    }

    if(flag == 1) {
        printf("1\n");
        char file_name_user_to_search[50];
        char dataToBeRead[1000];
        strcpy(file_name_user_to_search , user_to_search);
        strcat(file_name_user_to_search , ".user.json");
        FILE *filePointer;
        filePointer = fopen(file_name_user_to_search , "r");
        if(filePointer != NULL) {
            printf("2\n");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            printf("%s" , dataToBeRead);
            cJSON *root = cJSON_Parse(dataToBeRead);
            cJSON *username = cJSON_GetObjectItem(root, "username");
            printf("%s" , username->valuestring);
            cJSON *bio = cJSON_GetObjectItem(root, "bio");
            printf("%s" , bio->valuestring);
            cJSON *followers = cJSON_GetObjectItem(root, "followers");//want size
            cJSON *followings = cJSON_GetObjectItem(root, "followings");//want size
            cJSON *personalTweets = cJSON_GetObjectItem(root, "personalTweets");
            printf("3\n");
            //following status -> make for for client check followings
            cJSON *message = NULL;
            //---------------------------to find status
            char user_who_searchs_file_name[50];
            strcpy(user_who_searchs_file_name , online_username[i]);
            strcat(user_who_searchs_file_name , ".user.json");
            filePointer = fopen(user_who_searchs_file_name , "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(dataToBeRead);
            cJSON* followings_array = cJSON_GetObjectItem(root2 , "followings");
            int array_of_tweets_size = cJSON_GetArraySize(followings_array);
            int flag2=0;
            printf("4\n");
            for (i = 0; i < array_of_tweets_size; i++) {

                cJSON *elem = cJSON_GetArrayItem(followings_array, i);
                if (strcmp(elem->valuestring , user_to_search) == 0) {
                    flag2=1;
                    break;
                }
            }
            printf("5\n");
            message = cJSON_CreateObject();
            printf("%s" , cJSON_PrintUnformatted(username));
            cJSON_AddStringToObject(message, "username",  cJSON_PrintUnformatted(username));
            cJSON_AddStringToObject(message, "bio",  cJSON_PrintUnformatted(bio));
            cJSON_AddNumberToObject(message, "numberOfFollowers", cJSON_GetArraySize(followers));
            cJSON_AddNumberToObject(message, "numberOfFollowings",cJSON_GetArraySize(followings));
            if(flag2 == 1)
                cJSON_AddStringToObject(message, "followStatus", "Followed");
            else
                cJSON_AddStringToObject(message, "followStatus", "NotFollowed");

            cJSON_AddItemToObject(message, "allTweets", personalTweets);
            printf("7\n");
            cJSON *data_to_be_send = NULL;
            data_to_be_send = cJSON_CreateObject();

            cJSON_AddStringToObject(data_to_be_send, "type", "Profile");
            cJSON_AddItemToObject(data_to_be_send, "message",  message);
            printf("%s" ,  cJSON_PrintUnformatted(data_to_be_send) );
            send(client_socket , cJSON_PrintUnformatted(data_to_be_send) , strlen(cJSON_PrintUnformatted(data_to_be_send)) , 0);
        }
    }
}


void setbio(char buffer[1000] , int client_socket) {
    printf("0\n");
    char token_of_message[32];
    char bio[50];
    int i, j, flag = 0;
    sscanf(buffer, "set bio %s %s", token_of_message, bio);
    token_of_message[strlen(token_of_message) - 1] = '\0';

    for (i = 0; i < 100; i++) {
        if (strcmp(token[i], token_of_message) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 1) {
        char file_name_user[50];
        char dataToBeRead[1000];
        strcpy(file_name_user, online_username[i]);
        strcat(file_name_user, ".user.json");
        FILE *filePointer;
        filePointer = fopen(file_name_user, "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root = cJSON_Parse(dataToBeRead);
        cJSON *curent_bio = cJSON_GetObjectItem(root, "bio");

        if (strlen(curent_bio->valuestring) < 2) {
            curent_bio->valuestring = bio;

            filePointer = fopen(file_name_user, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root));
            fclose(filePointer);
            printf("bio changed\n");
        } else {
            printf("you cant change\n");
        }

    }
}

void logout(char buffer[1000]){
    char token_in_message[32];
    sscanf(buffer , "logout %s" , token_in_message);
    int i , flag=0;
    for(i=0 ; i<100 ; i++){
        if(strcmp(token[i] , token_in_message) == 0){
            flag = 1;
        }
    }
    if(flag == 0){
        printf("token is nit valid\n");
    } else{
        strcpy(token[i] , "");
        strcpy(online_username[i] , "");
    }

}

void follow(char  buffer[1000]){
    char token_in_message[32];
    char username_to_follow[20];
    sscanf(buffer , "follow %s %s" , token_in_message , username_to_follow);
    token_in_message[strlen(token_in_message) - 1] = '\0';
    int i , flag=0;
    printf("0\n");
    for(i=0 ; i<100 ; i++){
        if(strcmp(token_in_message , token[i]) == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        printf("token is not valid\n");
    }
    else{

        //check not to follow
        FILE *filePointer;
        char user_file_name[50];
        char dataToBeRead[1000];
        int flag2 = 0;
        strcpy(user_file_name , online_username[i]);
        strcat(user_file_name , ".user.json");
        filePointer = fopen(user_file_name , "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root1 = cJSON_Parse(dataToBeRead);
        cJSON*followings1 = cJSON_GetObjectItem(root1 , "followings");
        for(int j=0 ; j< cJSON_GetArraySize(followings1) ; j++){
            cJSON *elem = cJSON_GetArrayItem(followings1, i);
            if (strcmp(elem->valuestring , username_to_follow) == 0) {
                flag2=1;
                break;
            }
        }
        if(flag2 == 1){
            printf("you have followed this user before\n");
        } else {
            char file_name_to_follow[50];
            strcpy(file_name_to_follow, username_to_follow);
            strcat(file_name_to_follow, ".user.json");
            filePointer = fopen(file_name_to_follow, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root = cJSON_Parse(dataToBeRead);
            cJSON *followers = cJSON_GetObjectItem(root, "followers");
            cJSON_AddItemToArray(followers, cJSON_CreateString(online_username[i]));
            filePointer = fopen(file_name_to_follow, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root));
            fclose(filePointer);
            char file_name_follows[50];
            strcpy(file_name_follows, online_username[i]);
            strcat(file_name_follows, ".user.json");
            filePointer = fopen(file_name_follows, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(dataToBeRead);
            cJSON *followings = cJSON_GetObjectItem(root2, "followings");
            cJSON_AddItemToArray(followings, cJSON_CreateString(username_to_follow));
            filePointer = fopen(file_name_follows, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root2));
            fclose(filePointer);
            printf("%s\n", cJSON_PrintUnformatted(root2));
        }
    }
}



void profile(char buffer[1000] , int client_socket) {///////////////////////////////////has problem
    char token_of_message[32];
    int i, flag = 0;
    sscanf(buffer, "profile %s", token_of_message);
    // token_of_message[strlen(token_of_message) - 1] = '\0';
    printf("--- %s\n" , token_of_message);


    //search if token is valied?
    for(i=0 ; i<100 ; i++){
        if(strcmp(token[i] , token_of_message) == 0){
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        printf("invalid token\n");
    }

    if(flag == 1) {
        char file_name_user_to_search[50];
        char dataToBeRead[1000];
        strcpy(file_name_user_to_search , online_username[i]);
        strcat(file_name_user_to_search , ".user.json");
        FILE *filePointer;
        filePointer = fopen(file_name_user_to_search , "r");
        if(filePointer != NULL) {
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root = cJSON_Parse(dataToBeRead);
            cJSON *username = cJSON_GetObjectItem(root, "username");
            cJSON *bio = cJSON_GetObjectItem(root, "bio");
            cJSON *followers = cJSON_GetObjectItem(root, "followers");//want size
            cJSON *followings = cJSON_GetObjectItem(root, "followings");//want size
            cJSON *personalTweets = cJSON_GetObjectItem(root, "personalTweets");
            //following status -> make for for client check followings
            cJSON *message = NULL;
            //---------------------------to find status


            message = cJSON_CreateObject();
            cJSON_AddItemToObject(message, "username",  username);
            cJSON_AddItemToObject(message, "bio",  bio);
            cJSON_AddNumberToObject(message, "numberOfFollowers", cJSON_GetArraySize(followers));
            cJSON_AddNumberToObject(message, "numberOfFollowings",cJSON_GetArraySize(followings));


            cJSON_AddItemToObject(message, "allTweets", personalTweets);

            cJSON *data_to_be_send = NULL;
            data_to_be_send = cJSON_CreateObject();

            cJSON_AddStringToObject(data_to_be_send, "type", "Profile");
            cJSON_AddItemToObject(data_to_be_send, "message",  message);

            send(client_socket , cJSON_PrintUnformatted(data_to_be_send) , strlen(cJSON_PrintUnformatted(data_to_be_send)) , 0);
        }
    }
}



void unfollow(char buffer[1000]) {
    char token_in_message[32];
    char username_to_unfollow[20];
    sscanf(buffer, "unfollow %s %s", token_in_message, username_to_unfollow);
    token_in_message[strlen(token_in_message) - 1] = '\0';
    int i, flag = 0, j = 0;
    printf("0\n");
    for (i = 0; i < 100; i++) {
        if (strcmp(token_in_message, token[i]) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        printf("invalid token\n");
    }
    if (flag == 1) {
        printf("1\n");
        FILE *filePointer;
        int flag2 = 0;
        char file_name_to_unfollow[50];
        char dataToBeRead[1000];
        char user_file_name[50];
        strcpy(user_file_name, online_username[i]);
        strcat(user_file_name, ".user.json");
        filePointer = fopen(user_file_name, "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root1 = cJSON_Parse(dataToBeRead);
        cJSON *followings1 = cJSON_GetObjectItem(root1, "followings");
        for (j = 0; j < cJSON_GetArraySize(followings1); j++) {
            cJSON *elem = cJSON_GetArrayItem(followings1, i);
            if (strcmp(elem->valuestring, username_to_unfollow) == 0) {
                flag2 = 1;
                break;
            }
        }
        if (flag2 == 0) {
            printf("you have not followed this user before\n");
        }
        if (flag2 == 1) {
            strcpy(file_name_to_unfollow, username_to_unfollow);
            strcat(file_name_to_unfollow, ".user.json");
            filePointer = fopen(file_name_to_unfollow, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root = cJSON_Parse(dataToBeRead);
            cJSON *followers = cJSON_GetObjectItem(root, "followers");
            for (i = 0; i < cJSON_GetArraySize(followers); i++) {
                cJSON *subitem = cJSON_GetArrayItem(followers, i);
                if (strcmp(online_username[i], subitem->valuestring) == 0) {
                    break;
                }
            }
            cJSON_DeleteItemFromArray(followers, j);
            filePointer = fopen(file_name_to_unfollow, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root));
            fclose(filePointer);
            //cJSON *root = cJSON_Parse(dataToBeRead);
            printf("2\n");
            char file_name_unfollows[50];
            strcpy(file_name_unfollows, online_username[i]);
            strcat(file_name_unfollows, ".user.json");
            filePointer = fopen(file_name_unfollows, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(dataToBeRead);
            cJSON *followings = cJSON_GetObjectItem(root2, "followings");
            for (j = 0; j < cJSON_GetArraySize(followings); j++) {
                cJSON *subitem = cJSON_GetArrayItem(followings, j);
                if (strcmp(username_to_unfollow, subitem->valuestring) == 0) {
                    break;
                }
            }
            cJSON_DeleteItemFromArray(followings, j);
            filePointer = fopen(file_name_unfollows, "w");
            fprintf(filePointer, "%s", cJSON_PrintUnformatted(root2));
            fclose(filePointer);
            printf("%s\n", cJSON_PrintUnformatted(root2));
            printf("3\n");

        }
    }
}
void refresh(char buffer[1000] , int client_socket){
    char token_in_message[32];
    sscanf(buffer, "refresh %s", token_in_message);
    int i, flag = 0 , j , x;
    printf("0\n");
    for (i = 0; i < 100; i++) {
        if (strcmp(token_in_message, token[i]) == 0) {
            flag = 1;
            break;
        }
    }
    if(flag == 1) {
        printf("wewe\n");
        FILE *filePointer;
        char dataToBeRead[1000];
        char file_name[50];

        strcpy(file_name, online_username[i]);
        strcat(file_name, ".user.json");
        filePointer = fopen(file_name, "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root = cJSON_Parse(dataToBeRead);
        cJSON *followings = cJSON_GetObjectItem(root, "followings");
        cJSON *followers = cJSON_GetObjectItem(root, "followers");
        cJSON *data_to_send = cJSON_CreateArray();
        char name_to_get_tweets[50];
        printf("asdads\n");

        for (i = 0; i < cJSON_GetArraySize(followings); i++) {//followings
            printf("adad\n");
            cJSON *tweet_names = cJSON_GetArrayItem(followings, i);
            strcpy(name_to_get_tweets, tweet_names->valuestring);
            strcat(name_to_get_tweets, ".user.json");
            printf("----- %s --------\n" , name_to_get_tweets);
            char data_to_read_followings[1000];
            filePointer = fopen(name_to_get_tweets, "r");
            fgets(data_to_read_followings, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(data_to_read_followings);
            cJSON *allTweets = cJSON_GetObjectItem(root2, "personalTweets");
            printf("12\n");
            printf("%d\n" ,  cJSON_GetArraySize(allTweets));

            for (j = 0; j < cJSON_GetArraySize(allTweets); j++) {
                int flag3=0;

                printf("parda\n");

                cJSON *tweet_info = cJSON_GetArrayItem(allTweets, i);
                cJSON *tweet_id_info = cJSON_GetObjectItem(tweet_info , "id");
                for( x=0 ; x< 100 ; x++){
                    printf("1. %d\n" , tweet_id_info->valueint);
                    if(seen_tweet_ids[i][x] == -1){
                        break;
                    }
                    if(seen_tweet_ids[i][x] == tweet_id_info->valueint){
                        flag3 = 1;

                        break;
                    }
                }
                if(flag3 == 0){
                    printf("2222222");
                    // cJSON_AddItemToArray(tweet_id_seen_refresh , tweet_id_info);
                    // printf("%s" , cJSON_PrintUnformatted(tweet_id_seen_refresh));
                    seen_tweet_ids[i][x] = tweet_id_info->valueint;
                    cJSON_AddItemToArray(data_to_send, tweet_info);
                    printf("%s\n" , cJSON_PrintUnformatted(data_to_send));


                    printf("%s\n" , cJSON_PrintUnformatted(data_to_send));

                }

                else{
                    flag3 = 0;
                }
                printf("%s\n" , cJSON_PrintUnformatted(data_to_send));
                printf("j is : %d" , j);
            }
            printf("%s\n" , cJSON_PrintUnformatted(data_to_send));

        }


        for (i = 0; i < cJSON_GetArraySize(followers); i++) {
            printf("45\n");
            cJSON *tweet_names = cJSON_GetArrayItem(followers, i);
            strcpy(name_to_get_tweets, tweet_names->valuestring);
            strcat(name_to_get_tweets, ".user.json");
            printf("----- %s --------\n" , name_to_get_tweets);

            filePointer = fopen(name_to_get_tweets, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(dataToBeRead);
            cJSON *allTweets = cJSON_GetObjectItem(root2, "personalTweets");
            printf("33\n");
            for (j = 0; j < cJSON_GetArraySize(allTweets); j++) {
                printf("parda\n");
                cJSON *tweet_info = cJSON_GetArrayItem(allTweets, i);
                cJSON_AddItemToArray(data_to_send, tweet_info);
            }

        }
        printf("%s\n" , cJSON_PrintUnformatted(data_to_send));
      //  filePointer = fopen(file_name, "w");
        //fprintf(filePointer, "%s" , cJSON_PrintUnformatted(root));
        //fclose(filePointer);
        cJSON *send_to_client = NULL;
        send_to_client = cJSON_CreateObject();
        cJSON_AddStringToObject(send_to_client, "type", "List");
        printf("%s\n" , cJSON_PrintUnformatted(data_to_send));

        cJSON_AddItemToObject(send_to_client, "message", data_to_send);
        printf("%s\n" , cJSON_PrintUnformatted(data_to_send));

        char *string = cJSON_PrintUnformatted(send_to_client);
        printf("%s\n", string);
        send(client_socket, string, strlen(string), 0);

    }
    else{
        printf("token is not valid\n");
    }
}



void changepassword(char buffer[1000]){
    char curentpassword[20];
    char newpassword[16];
    char token_in_message[32];
    char hold[50];
    printf("%s\n" , buffer);
    sscanf(buffer , "changepassword %s %s " , token_in_message , newpassword);
    printf("%s\n" , token_in_message );
    // printf("%s\n" , newpassword);
    // for(int f=0 ; f<strlen(newpassword) ; f++){
    //   token_in_message[strlen(token_in_message) - 1 -f] = '\0';

    //}
    //printf("eeeee %s\n" , curentpassword);
    printf("%s\n" , newpassword);
    //token_in_message[strlen(token_in_message) - 1] = '\0';
    printf("%s" , token_in_message );
    //   curentpassword[strlen(curentpassword) - 1] = '\0';
    printf("%s\n" , newpassword);

    int i, flag = 0, j = 0;
    printf("0\n");
    printf("%s\n" , newpassword);

    for (i = 0; i < 100; i++) {
        strcpy(hold , token[i]);
        strcat(hold , newpassword);
        if (strcmp(token_in_message, hold) == 0) {
            flag = 1;
            break;
        }
    }
    printf("%s\n" , newpassword);

    if (flag == 0) {
        printf("invalid token\n");
    }
    if (flag == 1) {
        if(strlen(newpassword) > 16){
            printf("bad format request new pass is too long\n");
        }
        else{
            printf("1\n");
            printf("%s\n" , newpassword);

            FILE *filePointer;
            char dataToBeRead[1000];
            char file_name[50];
            printf("%s\n" , newpassword);

            strcpy(file_name, online_username[i]);
            strcat(file_name, ".user.json");
            filePointer = fopen(file_name, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            printf("%s\n" , newpassword);

            cJSON *root = cJSON_Parse(dataToBeRead);
            printf("%s\n" , newpassword);

            cJSON_DeleteItemFromObject(root , "password");
            printf("%s\n" , newpassword);
            cJSON_AddStringToObject(root , "password" , newpassword);
            // cJSON*pass = cJSON_GetObjectItem(root , "password");
            // strcpy(pass->valuestring , newpassword);
            //  printf("-----%s\n" , pass->valuestring );
            //cJSON_ReplaceItemInObject(root , "password" , cJSON_Parse(newpassword));
            // cJSON_AddStringToObject(root , "bio" , newpassword);
            printf("%s\n" , cJSON_PrintUnformatted(root));
            //  strcat(file_name, ".user.json");
            filePointer = fopen(file_name, "w");
            fprintf(filePointer, "%s" , cJSON_PrintUnformatted(root));
            fclose(filePointer);
            printf("password changed\n");
        }
    }

}

void deletetweet(char buffer[1000]){
    printf("qwe\n");
    char token_in_message[32];
    int tweet_id_to_delete = 0;
    sscanf(buffer , "deletetweet %s%d" , token_in_message , &tweet_id_to_delete);
    token_in_message[strlen(token_in_message) - 1] = '\0';
    printf("%d" , tweet_id_to_delete);
    printf("%s" , token_in_message);
    int i, flag = 0, j = 0;
    // printf("0\n");
    for (i = 0; i < 100; i++) {
        if (strcmp(token_in_message, token[i]) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        printf("invalid token\n");
    }
    if (flag == 1) {
        char tweet_to_delete_file_name[50];
        char writer_file_name[50];
        char dataToBeRead[1000];
        sprintf(tweet_to_delete_file_name , "%d" , tweet_id_to_delete);
        strcat(tweet_to_delete_file_name , ".tweet.json");
        FILE *filePointer;
        filePointer = fopen(tweet_to_delete_file_name, "r");
        fgets(dataToBeRead, 1000, filePointer);
        fclose(filePointer);
        cJSON *root = cJSON_Parse(dataToBeRead);
        cJSON*author = cJSON_GetObjectItem(root , "author");
        cJSON*is_deleted = cJSON_GetObjectItem(root , "is_deleted");
        cJSON_SetIntValue(is_deleted,  1);
        filePointer = fopen(tweet_to_delete_file_name, "w");
        fprintf(filePointer, "%s" , cJSON_PrintUnformatted(root));
        fclose(filePointer);

        if(strcmp(author->valuestring , online_username[i]) == 0){
            char writer_file_name[50];
            strcpy(writer_file_name , online_username[i]);
            strcat(writer_file_name , ".user.json");
            filePointer = fopen(writer_file_name, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root2 = cJSON_Parse(dataToBeRead);
            cJSON*personalTweets = cJSON_GetObjectItem(root2 , "personalTweets");
            for(j=0 ; j< cJSON_GetArraySize(personalTweets) ; j++){
                cJSON *tweets = cJSON_GetArrayItem(personalTweets, i);
                cJSON*tweet_writer = cJSON_GetObjectItem(tweets , "id");
                if(tweet_writer->valueint == tweet_id_to_delete){
                    break;
                }
            }
            cJSON_DeleteItemFromArray(personalTweets , j);

            filePointer = fopen(writer_file_name, "w");
            fprintf(filePointer, "%s" , cJSON_PrintUnformatted(root2));
            fclose(filePointer);
            printf("tweet deleted!!\n");

        }
        else{
            printf("tweet is not yours!!\n");
        }

    }

}



void searchtweet(char buffer[1000] , int client_socket){
    char token_in_message[32];
    char string_to_find[50];
    sscanf(buffer , "searchtweet %s%s" , token_in_message , string_to_find);
    token_in_message[strlen(token_in_message) - 1] = '\0';
    int i, flag = 0, j , flag2=0;
    // printf("0\n");
    for (i = 0; i < 100; i++) {
        if (strcmp(token_in_message, token[i]) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        printf("invalid token\n");
    }
    if (flag == 1) {
        FILE *filePointer;
        char tweet_file_name[50];
        char dataToBeRead[1000];
        cJSON * tweets_to_send = cJSON_CreateArray();
        for(j=0 ; ; j++){
            sprintf(tweet_file_name , "%d.tweet.json" , j);
            filePointer = fopen(tweet_file_name, "r");
            if(filePointer == NULL){
                break;
            }
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON *root = cJSON_Parse(dataToBeRead);
            cJSON*is_deleted = cJSON_GetObjectItem(root , "is_deleted");
            if(is_deleted->valueint == 0){
                cJSON*content = cJSON_GetObjectItem(root , "content");
                char*car1 = cJSON_PrintUnformatted(content);

                for(int x=0 ; x<strlen(car1) ; x++){
                    for(int y=0 ; y<strlen(string_to_find) ; y++){
                        if(car1[x+y] != string_to_find[y]){
                            flag2 = 1;
                            break;
                        }
                    }
                    if(flag2 == 0){
                        cJSON_AddItemToArray(tweets_to_send , root);
                    }
                    if(flag2 == 1){
                        flag2 = 0;
                    }

                }

            }
        }
        //-----------------------
        cJSON*data = cJSON_CreateObject();
        cJSON_AddStringToObject(data , "type" , "searchedtweet");
        cJSON_AddItemToObject(data , "message" , tweets_to_send);
        send(client_socket , cJSON_PrintUnformatted(data) , strlen(cJSON_PrintUnformatted(data)) , 0);

    }

}


void seecomments(char buffer[1000] , int client_socket){
    char token_in_message[32];
    char string_to_find[50];
    sscanf(buffer , "seecomments %s" , token_in_message );
    int i, flag = 0, j , flag2=0;
    // printf("0\n");
    for (i = 0; i < 100; i++) {
        if (strcmp(token_in_message, token[i]) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        printf("invalid token\n");
    }
    if (flag == 1) {
        cJSON * message = cJSON_CreateArray();
        char tweet_file_name[50];
        char dataToBeRead[1000];
        FILE *filePointer;

        for(j=0 ; ; j++){
            if(seen_tweet_ids[i][j] == -1){
                break;
            }
            sprintf(tweet_file_name , "%d.tweet.json" , seen_tweet_ids[i][j]);
            filePointer = fopen(tweet_file_name, "r");
            fgets(dataToBeRead, 1000, filePointer);
            fclose(filePointer);
            cJSON*root = cJSON_Parse(dataToBeRead);
            cJSON_AddItemToArray(message , root);
        }
        cJSON * data = cJSON_CreateObject();
        cJSON_AddStringToObject(data , "type" , "comments");
        cJSON_AddItemToObject(data , "message" , message);
        printf("%s\n" , cJSON_PrintUnformatted(data)  );
        send(client_socket , cJSON_PrintUnformatted(data) , strlen( cJSON_PrintUnformatted(data)) , 0);

    }
}

/*
int main(void){//تعداد شریف در یک رشته
    char car1[200];
    char check[7] = "Sharif";
    int i , count=0 , j , flag=0;
    gets( car1);
    for( i=0 ; i<strlen(check) ; i++){
        check[i] = tolower(check[i]);
    }
    for(i=0 ; i<strlen(car1) ; i++){
        car1[i] = tolower(car1[i]);
    }
    for(i=0 ; i<strlen(car1) ; i++){
        for(j=0 ; j<strlen(check) ; j++){
            if(car1[i+j] != check[j]){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            count++;
        }
        else{flag=0;}
    }

    printf("%d" , count);
}
*/