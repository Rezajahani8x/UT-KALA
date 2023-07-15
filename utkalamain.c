#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "utkalalib.h"

//==========================================================================================

//Main

int main() {

    printf("|   UT-Kala   |\n");

    User* users;
    Product* products;
    users = (User*)malloc(2 * sizeof(User));
    products = (Product*)malloc(2 * sizeof(Product));
    int len_users = 2;
    int len_products = 2;
    //dynamic memory allocation for saving datas about products and users

    if (users == NULL || products == NULL) {
        printf("Can't allocate memory!\n");
    }

    //inputs by user:
    /*
    1-signup <username> <password> <role>  2-login <username> <password> <role>  3-logout <>  4-view <>
    5-deposit <money>    6-add_goods <goods_name> <price> <count>  7-show_goods <>   8-buy <goods_name> <count> <sellers_name>
    */

    char command[15];

    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "signup")) {
            //sign up
            //asking data from user
            char* username = (char*)malloc(1 * sizeof(char));
            char* password = (char*)malloc(1 * sizeof(char));
            char role[10];
            int  r;
            getchar();//get a space : ' '
            username = get_string(username);
            password = get_string(password);
            scanf(" %s", role);
            //allocating memory for strings in user's data base
            int l;
            l = strlen(username);
            users[len_users - 1].name = (char*)malloc((l + 1) * sizeof(char)); //need to be checked
            l = strlen(password);
            users[len_users - 1].password = (char*)malloc((l + 1) * sizeof(char));
            //sending data to the function
            r = signup(users, username, password, role, len_users);
            if (r != -1) {
                users[len_users - 1].products = (Product*)malloc(2 * sizeof(Product));
                users = (User*)realloc(users, (len_users + 1) * sizeof(User));
                len_users++;
            }
            //users are saved in users[1] to users[i] with name / password / role / products
            //free what was allocated for temporary needs!
            free(username);
            free(password);
            continue;
        }
        else if (!strcmp(command, "login")) {
            //login
            //asking data from user
            char* username = (char*)malloc(1 * sizeof(char));
            char* password = (char*)malloc(1 * sizeof(char));
            char role[10];
            getchar();//get the space ' ';
            username = get_string(username);
            password = get_string(password);
            scanf(" %s", role);
            //User's data should be check to see if user can log in;
            int r;
            r = login(users, username, password, role, len_users);
            int user_num; //user number helps us to increase speed of coding
            user_num = user_num_calculater(users, username, role, len_users);
            //free what was allocated temporary
            free(username);
            free(password);
            //loging in and other stuff ... 
            if (r == -1) {
                //user cant login!
                continue;
            }
            if (r == 0) {
                //user can login
                //inputs by the user who has loged in
            /*
            3-logout <> 4-view <> 5-deposit <money> 6-add_goods <goods_name> <price> <count>
            7-show_goods <>  8-buy <goods_name> <count> <sellers_name>
            */

                while (1) {
                    scanf("%s", command);
                    if (strcmp(command, "logout") == 0) {
                        //user logs out
                        if (command[6] != '\0') {
                            //wrong command
                            printf("Error!Can't run the command!\n");
                            continue;
                        }
                        else {
                            printf("Loged out!\n");
                            break;
                        }
                    }
                    if (strcmp(command, "view") == 0) {
                        //view , the user can see his/her profile
                        if (command[4] != '\0') {
                            printf("Error!Can't run the command!\n");
                            continue;
                        }
                        else {
                            view(users, user_num);
                        }
                        continue;
                    }
                    if (strcmp(command, "deposit") == 0) {
                        //deposit money , user should be only buyer
                        if (users[user_num].role[0] != 'B') {//user aint a buyer
                            printf("Error!Only buyers can deposit!\n");
                            continue;
                        }
                        int deposit_money;
                        scanf(" %d", &deposit_money);
                        users[user_num].money += deposit_money;
                        printf("You have deposited your money!\n");
                        continue;
                    }
                    if (strcmp(command, "add_goods") == 0) {//user can add products if and only if user is seller!
                        //add goods
                        //asking data from user
                        char* goods_name = (char*)malloc(1 * sizeof(char));
                        int price;
                        int count;
                        getchar();//get a space ' ';
                        goods_name = get_string(goods_name);
                        scanf("%d %d", &price, &count);
                        int r;
                        //allocating necessary memory for some data
                        r = exist_check_good(products, goods_name, len_products);
                        if (r == 0 && users[user_num].role[0] == 'S') {
                            int le;
                            le = strlen(goods_name);
                            products[len_products - 1].name = (char*)malloc((le + 1) * sizeof(char));
                            users[user_num].products[users[user_num].len_of_product - 1].name = (char*)malloc((le + 1) * sizeof(char));
                            le = strlen(users[user_num].name);
                            products[len_products - 1].seller_name = (char*)malloc((le + 1) * sizeof(char));
                        }
                        //process of adding the good
                        r = add_goods(users, products, user_num, goods_name, price, count, len_products);
                        if (r == 1) {
                            int l = users[user_num].len_of_product;
                            products = (Product*)realloc(products, (len_products + 1) * sizeof(Product));
                            users[user_num].products = (Product*)realloc(users[user_num].products, (l + 1) * sizeof(Product));
                            users[user_num].len_of_product++;
                            len_products++;
                        }
                        free(goods_name);
                        continue;
                    }
                    if (strcmp(command, "show_goods") == 0) {//command for displaying all of the available products
                        if (command[10] != '\0') {
                            //Error
                            printf("Error!Can't run the command!\n");
                            continue;
                        }
                        show_goods(products, len_products);
                        continue;
                    }
                    if (strcmp(command, "buy") == 0) {//buy,only buyer users can run the command
                        //buy
                        //asking data from user
                        char* goods_name = (char*)malloc(1 * sizeof(char));
                        int count;
                        char* sellers_name = (char*)malloc(1 * sizeof(char));
                        getchar();//get a space
                        goods_name = get_string(goods_name);
                        scanf(" %d ", &count);
                        sellers_name = get_string(sellers_name);
                        //allocating necessary memory for buyer products list
                        int r;
                        r = check_for_buy(users, products, goods_name, count, sellers_name, user_num, len_users, len_products);
                        if (r == 0) {
                            int lle = users[user_num].len_of_product;
                            int le;
                            le = strlen(goods_name);
                            users[user_num].products[lle - 1].name = (char*)malloc((le + 1) * sizeof(char));
                            le = strlen(sellers_name);
                            users[user_num].products[lle - 1].seller_name = (char*)malloc((le + 1) * sizeof(char));
                        }
                        //process of buying
                        r = buy(users, products, goods_name, count, sellers_name, user_num, len_users, len_products);
                        if (r == 1) {
                            int l_b = users[user_num].len_of_product;
                            //now we should extend the size of purchased goods for buyer
                            users[user_num].products = (Product*)realloc(users[user_num].products, (l_b + 1) * sizeof(Product));
                            users[user_num].len_of_product++;
                        }
                        //free temporary allocated memory
                        free(goods_name);
                        free(sellers_name);
                        continue;
                    }
                    if (!strcmp(command, "signup") || !strcmp(command, "login")) {
                        printf("Error!User is loged in no such command is allowed!\n");
                        continue;
                    }
                    else {
                        //Error
                        printf("Error!Can't run the command!\n");
                        continue;
                    }
                }
                continue;
            }
        }
        else if (!strcmp(command, "logout") || !strcmp(command, "view") || !strcmp(command, "deposit") || !strcmp(command, "add_goods") || !strcmp(command, "show_goods") || !strcmp(command, "buy")) {
            printf("Error!No user has loged in!\n");
            continue;
        }
        else if (!strcmp(command, "Exit")) {
            //user can exit the app
            printf("You have successfully quited the program!Thanks for visiting UT-Kala!\n");
            break;
        }
        else if (!strcmp(command, "logout") || !strcmp(command, "view") || !strcmp(command, "add_goods") || !strcmp(command, "show_goods") || !strcmp(command, "buy") || !strcmp(command, "deposit")) {
            printf("Error!No user has loged in!\n");
            continue;
        }
        else {
            //Error!command cant be read
            printf("Error!Can't read the command!\n");
        }
    }

    free(users);
    free(products);

    return 0;
}