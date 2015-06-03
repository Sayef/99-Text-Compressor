#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

int I_glob = 0, FLAG = 1;    //eita use kora hoise struct tree_represen s[] er index er jonno.

char C_A_compress_file_name[30], input_txt[50], compress_txt[50], key_txt[50];  // jei file compress korte hobe sei file name
int I_com_file_size = 0;

struct tree_represen    //eita use kora hoise tree representation (0 or 1) korar jonno
{
    int I_max_frequen;
    char C_prechar;
    string STR_encode;
}s[256], s1;

struct S_mytree    //eita use kora hoise binary tree er node gulo create korar jonno
{
    int I_max;
    char C_charasci;
    S_mytree *S_P_left, *S_P_right, *S_P_mid;
};

/* ei function er kaj holo proti ti character
 er jonno node toiri kora and node gulo
 eder frequency onujai sort kora.
 jar man kom se uporer dike thakbe*/

void F_insertsort(S_mytree *S_M_head, int max, int asci)
{
    S_mytree *temp, *premem, *newnode;
    premem = S_M_head;
    temp = S_M_head;
    while(temp->S_P_mid != NULL)
    {
        temp = temp->S_P_mid;
        if(temp->I_max > max) break;
        premem = temp;
    }
    newnode = (S_mytree*) malloc(sizeof(S_mytree));
    newnode->C_charasci = asci;
    newnode->I_max = max;
    newnode->S_P_right = NULL;
    newnode->S_P_left = NULL;
    newnode->S_P_mid = premem->S_P_mid;
    premem->S_P_mid = newnode;
    return ;
}

/* ei function er kaj holo binary node gulo
 nie single node er jotha sthane sort kore boshano */

void F_insert(S_mytree *S_M_head, S_mytree *S_new)
{
    S_mytree *temp, *premem;
    temp = S_M_head;
    premem = S_M_head;
    while(temp->S_P_mid != NULL)
    {
        temp = temp->S_P_mid;
        if(temp->I_max > S_new->I_max) break;
        premem = temp;
    }
    S_new->S_P_mid = premem->S_P_mid;
    premem->S_P_mid = S_new;
    return ;
}

/* ei function er kaj holo ekti binary tree create kora*/

void F_create_tree(S_mytree *S_M_head)
{
    if(S_M_head->S_P_mid->S_P_mid == NULL) return ;
    else{
        int max_left, max_right;
        S_mytree *S_new, *temp;
        S_new = (S_mytree*) malloc(sizeof(S_mytree));
        temp = S_M_head;
        S_new->S_P_left = temp->S_P_mid;
        temp = temp->S_P_mid;
        max_left = temp->I_max;
        S_new->S_P_right = temp->S_P_mid;
        temp = temp->S_P_mid;
        max_right = temp->I_max;
        S_M_head->S_P_mid = temp->S_P_mid;
        S_new->I_max = max_left + max_right;
        F_insert(S_M_head, S_new);
        F_create_tree(S_M_head);
    }
}

/* ei function er kaj holo binary tree traversal kore
0 or 1 ber kore dewa.
 ar ekhane left mane 0 & right mane 1 */

void F_tree_traversal(S_mytree *top, string STR_temp)
{
    if(top->S_P_left == NULL && top->S_P_right == NULL)
    {
        s[I_glob].C_prechar = top->C_charasci;
        s[I_glob].STR_encode = STR_temp;
        s[I_glob].I_max_frequen = top->I_max;
        I_glob ++;
        return ;
    }
    F_tree_traversal(top->S_P_left, STR_temp + "0");
    F_tree_traversal(top->S_P_right, STR_temp + "1");
}

map< string , int > Map;       //eita holo binary to desimal convert korar jonno

map< int , string > Map2, Map1;  /*Map1 use for reverse of Map and Map2 use kora hoise
								 F_stru_re_cre te ekhane binary representasion rakha hoise*/

/*ei function er kaj holo 0 - 255 porjonto 8 bits kore
protitir binary number memory kora*/

void bin_con_des()
{
	int i, iFbin = 0;
	string binTOdes;
	binTOdes = "00000000";
	while(1)
	{
		Map[binTOdes] = iFbin;
		Map1[iFbin] = binTOdes;
		if(iFbin == 255) break;
		iFbin ++;
		for(i = 7; i >= 0; i --)
		{
			if(binTOdes[i] == '0')
			{
				binTOdes[i] = '1';
				break;
			}
			else binTOdes[i] = '0';
		}
	}
	return;
}

/*eita structure theke Map2 te
binary representation gula nia ashe*/

void F_stru_re_crea()
{
	int i, j;
	for(i = 0; i < I_glob; i ++)
	{
		j = s[i].C_prechar;
		Map2[j] = s[i].STR_encode;
	}

	return ;
}

void F_compress()
{
	char ch;
	int a, l, i;
	ch = I_com_file_size % 8;
	string S_chek, S_temp;
	F_stru_re_crea();
	FILE *P_file, *P_in;
	P_file = fopen(compress_txt , "wb");
	P_in = fopen(input_txt , "r");
	fprintf(P_file , "%c", ch);
	while(fscanf(P_in , "%c", &ch) == 1)
	{
		a = ch;
		S_chek = S_chek + Map2[a];
		if(S_chek.length() >= 8)
		{
			S_temp = S_chek;
			S_temp.erase(8 , S_temp.size());
			ch = Map[S_temp];
			fprintf(P_file , "%c", ch);
			S_chek.erase(0 , 8);
		}
	}
	while(S_chek.size() > 8)
	{
		S_temp = S_chek;
		S_temp.erase(8 , S_temp.size());
		ch = Map[S_temp];
		fprintf(P_file , "%c", ch);
		S_chek.erase(0 , 8);
	}
	if(S_chek.size() != 0)
	{
		l = S_chek.size();
		for(i = l; i < 8; i ++)
			S_chek += "0";
		ch = Map[S_chek];
		fprintf(P_file , "%c", ch);
		I_com_file_size += (8 - l);
	}
	fclose(P_file);
	fclose(P_in);

	return ;
}

// file decompress korar main function

void F_decompress(S_mytree *S_M_head)
{
	S_mytree *top;
	FILE *p_key, *p_decom;
	int i, maxi, a, b, numOFchar = -1;
	char ch;
	p_key = fopen(compress_txt , "rb");
	if(!p_key)
	{
	    printf("\nUnable to open the compress file or Not found the file\n\n");
	    FLAG = 0;
	    return ;
	}
	while(fscanf(p_key , "%c", &ch) == 1)
        numOFchar ++;
    fclose(p_key);
    p_decom = fopen(input_txt , "w");
    p_key = fopen(compress_txt , "rb");
	string S_chek;
	fscanf(p_key , "%c", &ch);
	b = ch;
    top = S_M_head->S_P_mid;
    while(fscanf(p_key , "%c", &ch) == 1)
    {
        a = ch;
		if(a < 0) a = 256 + a;
        S_chek = Map1[a];
        for(i = 0; i < 8; i ++)
        {
            if(S_chek[i] == '0')
                top = top->S_P_left;
            else top = top->S_P_right;
            if(top->S_P_left == NULL && top->S_P_right == NULL)
            {
                ch = top->C_charasci;
                fprintf(p_decom , "%c", ch);
                top = S_M_head->S_P_mid;
            }
        }
        numOFchar --;
        if(numOFchar == 1) break;
    }
    fscanf(p_key , "%c", &ch);
    a = ch;
	if(a < 0) a = 256 + a;
    S_chek = Map1[a];
    if(b == 0) b = 8;
    for(i = 0; i < b; i ++)
    {
        if(S_chek[i] == '0')
            top = top->S_P_left;
        else top = top->S_P_right;
        if(top->S_P_left == NULL && top->S_P_right == NULL)
        {
            ch = top->C_charasci;
            fprintf(p_decom , "%c", ch);
            top = S_M_head->S_P_mid;
        }
    }
    fclose(p_key);
    fclose(p_decom);
    return ;
}

int main()
{
    int I_i, I_A_freque_char[260], n, i, max;
    int I_asci, I_main_file_char = 0, kfs = 0;
    char C_input, ch;
    FILE *F_P_input;
    S_mytree *S_M_head;
    S_M_head = (S_mytree*) malloc(sizeof(S_mytree));
    S_M_head->S_P_right = NULL;
    S_M_head->S_P_left = NULL;
    S_M_head->S_P_mid = NULL;
    bin_con_des();
    printf("  ! ! ! ! !  welcome  ! ! ! ! !\nThis is a text file compress program\n\nplease select any one:\n\n");
    printf("1. for compress the file\n2. for decompress the file\n0. for exit\n\n");
    scanf("%d", &n);
    if(n == 0) return 0;
    if(n != 1 && n != 2){
        printf("\nThis is invalid number\n\n");
        return 0;
    }
    if(n == 1){
        printf("please enter the file name: ");
        scanf("%s", C_A_compress_file_name);
        strcpy(input_txt , C_A_compress_file_name);
        strcpy(compress_txt , C_A_compress_file_name);
        strcpy(key_txt , C_A_compress_file_name);
        strcat(input_txt , ".txt");
        strcat(compress_txt , ".99");
        strcat(key_txt , "_key.99");
        F_P_input = fopen(input_txt , "r");
        if(!F_P_input)
        {
            printf("\nUnable to open the file or Not found the file\n\n");
            return 0;
        }
        memset(I_A_freque_char, 0, sizeof(I_A_freque_char));
        while(fscanf(F_P_input, "%c", &C_input) == 1)
        {
            I_asci = C_input;
            I_A_freque_char[I_asci] ++;
            I_main_file_char ++;
        }
        fclose(F_P_input);
        F_P_input = fopen(key_txt , "wb");
        for(I_i = 0; I_i < 256; I_i ++)
        {
            if(I_A_freque_char[I_i] != 0){
                max = I_A_freque_char[I_i];
                fprintf(F_P_input , "%c%d,", I_i, max);
                F_insertsort(S_M_head, max, I_i);
            }
        }
        fclose(F_P_input);
        F_P_input = fopen(key_txt , "rb");
        while(fscanf(F_P_input , "%c", &C_input) == 1)
            kfs += 8;
    }
    if(n == 2){
        printf("please enter the compress file name: ");
        scanf("%s", C_A_compress_file_name);
        strcpy(input_txt , C_A_compress_file_name);
        strcpy(compress_txt , C_A_compress_file_name);
        strcpy(key_txt , C_A_compress_file_name);
        strcat(input_txt , ".txt");
        strcat(compress_txt , ".99");
        strcat(key_txt , "_key.99");
        F_P_input = fopen(key_txt , "rb");
        if(!F_P_input)
        {
            printf("\nUnable to open the key file or Not found the file\n\n");
            return 0;
        }
        while(fscanf(F_P_input , "%c", &ch) == 1)
        {
            fscanf(F_P_input , "%d", &max);
            i = ch;
            F_insertsort(S_M_head, max, i);
            fscanf(F_P_input , "%c", &ch);
        }
        fclose(F_P_input);
    }
    F_create_tree(S_M_head);
    if(n == 1){
        S_mytree *top;
        top = S_M_head->S_P_mid;
        F_tree_traversal(top->S_P_left, "0");
        F_tree_traversal(top->S_P_right, "1");
        for(I_i = 0; I_i < I_glob; I_i ++)
            I_com_file_size += (s[I_i].STR_encode.size() * s[I_i].I_max_frequen);
        F_compress();
        I_com_file_size += kfs;
        I_main_file_char = I_main_file_char * 8;
        printf("\nSize of main file : %d\n", I_main_file_char);
        printf("Size of compressed file : %d\n", I_com_file_size);
        printf("Reduced tha size : %d\n", I_main_file_char - I_com_file_size);
        printf("Percentage of the saving memory : %0.3lf\n\n", (double) (I_main_file_char - I_com_file_size) * 100 / I_main_file_char);
    }
    if(n == 2) {
        F_decompress(S_M_head);
        if(FLAG)
            printf("\nSuccessfully decompress the file\n\n  * * *  thank you for using the program  * * *\n\n");
    }

    return 0;
}
