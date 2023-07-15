#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
UT kala

 * 2 types of user: 1-Buyer 2-Seller
 * Each Stage : 0-No Error 1-Error   User should be alerted!
 * What user can see in view:
   -Name -Role -Budget
   -
   if(buyer){ list of purchased products with (name/price/count/seller name)}
   if(seller){list of purchasable products with(name/price/count)}

 Stages ::
  1-sign up
       signup <username> <password> <role>;
  2-log in
       login <username> <password> <role>;
  3-log out
       log out <>;
  4-view
       view <>;
  5-deposit
   ^ (only for buyyers)
       deposit <amount_of_money>;
  6-add goods
   ^ (only for sellers)
   ^ (This command is also used for adding an available product)
       add_goods <goods_name> <goods_price> <goods_count>;
  7-show goods
   ^ (using this command leads to show of all the products available)
   ^ (format:) <    seller / name / price / count    >
       show_goods <>;
  8-buy
   ^ (list of products should be updated)
   ^ (view data of both sides of the contract should be updated <Budget/list of products>
       buy <goods_name> <goods_count> <seller_username>;

*/

//data structs

struct product {
    char* name;
    char* seller_name;
    int price;
    int count;
};

struct user {
    char* name;
    char* password;
    char role[10];
    int money;
    int len_of_product;//to determine how many products are available to show in product no other use!declared 2 at first
    struct product* products;
};

typedef struct product Product;
typedef struct user User;


//==========================================================================================

//Functions

int signup(User* users, char* username, char* password, char* role, int len) {
    int i;
    //len is size of users
    //datas should be saved in users[len-1]
    if (!strcmp(role, "Buyer") || !strcmp(role, "Seller")) {
        for (i = 1; i < len - 1; i++) {
            if (strcmp(username, users[i].name) == 0) {
                if (strcmp(role, users[i].role) == 0) {
                    //Error!Username has already been taken
                    printf("Error!Username has already been taken!\n");
                    return -1;
                }
            }
        }
    }
    else {
        //Error!Role can't be read!
        printf("Error!Can't read the role!\n");
        return -1;
    }
    strcpy(users[len - 1].name, username);
    strcpy(users[len - 1].password, password);
    strcpy(users[len - 1].role, role);
    users[len - 1].money = 0;
    users[len - 1].len_of_product = 2;
    printf("Sign up successfull!\n");
    return 0;
}

int login(User* users, char* username, char* password, char* role, int len) {
    //to check if user can log in
    int result = -1;//-1 means user can't login!
    int i;
    for (i = 1; i < len - 1; i++) {
        int condition1 = strcmp(username, users[i].name);
        int condition2 = strcmp(role, users[i].role);
        int condition3 = strcmp(password, users[i].password);
        if (!condition1 && !condition2 && !condition3) {
            result = 0;
            break;
        }
    }
    if (result == 0) {
        //login successfull!
        printf("Login successfull!\n");
    }
    else {
        //can't login
        printf("Error!Can't login!Username or password or role may not be correct!\n");
    }
    return result;
}

int user_num_calculater(User* users, char* username, char* role, int len) {
    //once a user logs in many commands are possible to run and to control them it is easier to work with a num than a name
    int i;
    int user_num = 0;
    for (i = 1; i < len - 1; i++) {
        int condition1 = strcmp(username, users[i].name);
        int condition2 = strcmp(role, users[i].role);
        if (!condition1 && !condition2) {
            user_num = i;
            break;
        }
    }
    return user_num;
}

int view(User* users, int user_num) {
    //user's profile name / role / money / products
    int type = strcmp(users[user_num].role, "Buyer");//if 0 buyer if 1 seller
    int i;
    printf(" *    Profile   * \n");
    printf(" Name : %s\n", users[user_num].name);
    printf(" Role : %s\n", users[user_num].role);
    printf(" Money : %d\n", users[user_num].money);
    printf(" Products : \n");
    int l = users[user_num].len_of_product;
    if (l != 2) {
        if (type == 0) {//user is buyer
            for (i = 1; i < l - 1; i++) {
                printf("%s %d %d %s\n", users[user_num].products[i].name, users[user_num].products[i].price, users[user_num].products[i].count, users[user_num].products[i].seller_name);
            }
        }
        else {//user is seller
            for (i = 1; i < l - 1; i++) {
                printf("%s %d %d\n", users[user_num].products[i].name, users[user_num].products[i].price, users[user_num].products[i].count);
            }
        }
    }
    return 0;
}

int add_goods(User* users, Product* products, int user_num, char* goods_name, int price, int count, int len_products) {
    if (strcmp(users[user_num].role, "Seller") != 0) {
        printf("Error!Can't run the command!User isn't a seller!\n");
        return -1;
    }
    //2 lists should be updated 1-products list 2-seller's profile
    int i, j;
    int type = 0;//defines whether product already exists(1) or not(0) 
    int l = users[user_num].len_of_product;
    //checking for error
    for (i = 1; i < len_products - 1; i++) {
        if (strcmp(goods_name, products[i].name) == 0) {
            if (strcmp(users[user_num].name, products[i].seller_name) == 1) {//name of sellers are not the same //Error
                printf("Error!Can't add the product.It already exists!\n");
                return -1;
            }
            else {//if its the same product then it's count should be added!
                if (price == products[i].price) {
                    type = 1;
                    products[i].count += count; //products list is updated time to update seller's profile
                    for (j = 1; j < l - 1; j++) {
                        if (strcmp(goods_name, users[user_num].products[j].name) == 0 && price == users[user_num].products[j].price) {
                            users[user_num].products[j].count += count;
                            break;
                        }
                    }
                    printf("You have increased number of your product!\n");
                    return 0;
                }
            }
        }
    }
    if (type != 1) {
        strcpy(products[len_products - 1].name, goods_name);
        strcpy(products[len_products - 1].seller_name, users[user_num].name);
        products[len_products - 1].price = price;
        products[len_products - 1].count = count;
        //products list is updated now its time for seller's profile
        strcpy(users[user_num].products[l - 1].name, goods_name);
        users[user_num].products[l - 1].price = price;
        users[user_num].products[l - 1].count = count;
        printf("You have added your product!\n");
        return 1;
    }
}

int exist_check_good(Product* products, char* goods_name, int len_products) {
    int i;
    if (len_products == 2) {
        return 0;
    }
    for (i = 1; i < len_products - 1; i++) {
        if (strcmp(goods_name, products[i].name) == 0) {//good exists!
            return -1;
        }
    }
    return 0;
}

int show_goods(Product* products, int len_products) {
    int i;
    printf("   *     Products    *\n");
    printf(" Name   |   Price   |   Count   |   Seller's Name\n");
    if (len_products == 2) {
        return -1;
    }
    for (i = 1; i < len_products - 1; i++) {
        printf("%s  |   %d  |   %d  |   %s\n", products[i].name, products[i].price, products[i].count, products[i].seller_name);
    }
    return 0;
}

int buy(User* users, Product* products, char* goods_name, int count, char* sellers_name, int user_num, int len_users, int len_products) {
    //user should be buyer + count_khode_good !=0 + money-price*count>0 
    //stages : 1-Money Transfer {buyer --> Seller}  2-List updates {products/buyer's profile/seller's profile}
    //first we figure out number of each part {buyer:user_num already available/seller:seller_num/good:goods_num}
    int seller_num = 0, good_num = 0;
    int i;//if good_num or seller_num be kept 0 means no seller was found with this product so Error
    for (i = 1; i < len_users - 1; i++) {
        if (strcmp(sellers_name, users[i].name) == 0) {
            if (users[i].role[0] = 'S') {//the name refers to a seller and contract can work out!
                seller_num = i;
                break;
            }
        }
    }
    for (i = 1; i < len_products - 1; i++) {
        if (strcmp(goods_name, products[i].name) == 0) {
            if (strcmp(products[i].seller_name, sellers_name) == 0) {
                good_num = i;
                break;
            }
        }
    }
    if (seller_num == 0 || good_num == 0) {//Error
        printf("Error!Seller or product couldn't be found!\n");
        return -1;
    }
    //checking for possible errors
    if (users[user_num].role[0] != 'B') {
        printf("Error!Purchase can't be confirmed!User is not a buyer!\n");
        return -1;
    }
    if (products[good_num].count - count < 0) {
        printf("Error!Purchase can't be confirmed!Product is not available!\n");
        return -1;
    }
    if (count * products[good_num].price > users[user_num].money) {
        printf("Error!Purchase can't be confirmed!User's money is not enough!\n");
        return -1;
    }
    //now stages of purchase can be taken into account since the system has checked for any possible error
    //stage 1 : Money Transfer   {buyer --> seller}
    int transfer_money;
    transfer_money = count * products[good_num].price;
    users[user_num].money = users[user_num].money - transfer_money;
    users[seller_num].money = users[seller_num].money + transfer_money;

    //stage 2 : list updates {1-products 2-Buyer profile 3-seller profile}
    //1-products : count should be subtracted
    products[good_num].count = products[good_num].count - count;
    //2-seller profile : count should be subtracted
    int l_s = users[seller_num].len_of_product;
    for (i = 1; i < l_s; i++) {
        if (strcmp(goods_name, users[seller_num].products[i].name) == 0) {
            users[seller_num].products[i].count = users[seller_num].products[i].count - count;
            break;
        }
    }
    //3-buyer profile : either it aint the first purchase or it is
    /*
    2-if it aint the first time we only should increase the count
    1-if it is the first time we should add a new slot with new datas
    */
    int t = 1;
    int l_b = users[user_num].len_of_product;
    for (i = 1; i < l_b; i++) {
        if (strcmp(goods_name, users[user_num].products[i].name) == 0) {//product exists in the profile list
            users[user_num].products[i].count += count;
            t = 2;
            printf("Purchase successfull!\n");
            return 0;
        }
    }
    if (t == 1) {//first time purchase
        strcpy(users[user_num].products[l_b - 1].name, goods_name);
        strcpy(users[user_num].products[l_b - 1].seller_name, sellers_name);
        users[user_num].products[l_b - 1].price = products[good_num].price;
        users[user_num].products[l_b - 1].count = count;
        printf("Purchase successfull!\n");
        return 1;
    }
}

int check_for_buy(User* users, Product* products, char* goods_name, int count, char* sellers_name, int user_num, int len_users, int len_products) {
    //this function is used to check if we are supposed to allocate memory for names in buy action
    int seller_num = 0, good_num = 0;
    int i;//if good_num or seller_num be kept 0 means no seller was found with this product so Error
    for (i = 1; i < len_users - 1; i++) {
        if (strcmp(sellers_name, users[i].name) == 0) {
            if (users[i].role[0] = 'S') {
                seller_num = i;
                break;
            }
        }
    }
    for (i = 1; i < len_products - 1; i++) {
        if (strcmp(goods_name, products[i].name) == 0) {
            if (strcmp(products[i].seller_name, sellers_name) == 0) {
                good_num = i;
                break;
            }
        }
    }
    if (seller_num == 0 || good_num == 0) {
        return -1;
    }
    //checking for possible errors
    if (users[user_num].role[0] != 'B') {
        return -1;
    }
    if (products[good_num].count - count < 0) {
        return -1;
    }
    if (count * products[good_num].price > users[user_num].money) {
        return -1;
    }
    int l_b = users[user_num].len_of_product;
    if (l_b != 2) {
        for (i = 1; i < l_b - 1; i++) {
            if (strcmp(goods_name, users[user_num].products[i].name) == 0) {//product exists in the profile list
                return -1;
            }
        }
    }
    return 0;
}

char* get_string(char* str) {
    //for forming these strings we need unlimited length array:
    //1-username 2-password 3-goods_name 4-seller_name
    int len = 1;//length of the string allocated so far
    int i = 0;
    while (1) {
        char c;
        c = getchar();
        if (c == ' ' || c == '\n') {//end of the string
            str[i] = '\0';
            break;
        }
        else {//getting string still in progress
            str[i] = c;
            str = (char*)realloc(str, (len + 1) * sizeof(char));
            len++;
            i++;
        }
    }
    return str;
}

