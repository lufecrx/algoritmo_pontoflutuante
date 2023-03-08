#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>

#define BIT31_MASK 0x80000000
#define EXP_MASK 0x7F800000
#define MANTISSA_MASK 0x007FFFFF


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void pedir_valores();
char* verifica_zero(char [], char []);
int eh_numero_valido(char*, int);
int verifica_operacao();
char* inverter_binario(char*);
int32_t extrai_expoente(int32_t);
int32_t extrai_sinal(int32_t);
void operacao_bin(char* , char*);

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "Portuguese");
    int resposta = 1;
    
    pedir_valores();	
	
	while(resposta == 1){
		printf("\n\nDeseja fazer outra operação?");
		printf("\n1 - Sim \n2 - Não\n");
		scanf("%d", &resposta);
	
		if(resposta == 1){
			system("cls");
			pedir_valores();
		}
		else if(resposta == 2){
			return 0;
		}
		else{
			system("cls");
			printf("\nResposta inválida!");
		}		
	}
	
	return 0;		
	
}

void pedir_valores(){
	char biX[33], biY[33];
	int validade = 0;
	
	//Qual operação vai ser realizada? Soma ou subtração? 
	int operacao = verifica_operacao();
		
	//Recebe valor de X e de Y
	while(validade == 0){
		printf("[X] - Valor binário de 32 bits: ");
		scanf("%s", biX);
		int tamX = strlen(biX);	
		printf("[Y] - Valor binário de 32 bits: ");
		scanf("%s", biY);
		int tamY = strlen(biY);	
		
		system("cls");   
   
		       
		//verificar validdde de X e Y
		if(eh_numero_valido(biX, tamX) && eh_numero_valido(biY, tamY)){
			validade++;
		} else{
			printf("\nATENÇÃO: Você está colocando valores inválidos como letras ou valores diferente de 1 ou 0,");
			printf("\nou pelo menos um dos valores não está correto. Lembre-se de que X e Y devem receber, cada um, exatamente 32 bits.");
			printf("\n\nX está com %d bits \nY está com %d bits \n\nInsira novamente os valores e tente mais uma vez:\n\n", tamX, tamY);
		}		
	}
	
	//X ou Y é igual a ZERO? 
	char* result = verifica_zero(biX, biY);
	
	// se X ou Y for igual a zero
	if(strlen(result) != 0) {
        printf("Resultado: %s \n", result);
		return;    
    } 
    
    //Se a operação for de subtração, mudar sinal de Y
    if(operacao == 2){
 	    char* binario_invertido_str = inverter_binario(biY);
    	strcpy(biY, binario_invertido_str);
    	free(binario_invertido_str);
	}
	
	printf("Valor de X: %s \n", biX);
	printf("Valor de Y: %s \n", biY); 
	
	operacao_bin(biX, biY);
	
}

char* verifica_zero(char biX[], char biY[]){
	int zeroX = 1, zeroY = 1, i;
	
	for(i = 1; i<32; i++){
		if(biX[i] != '0'){
			zeroX = 0;					
		}
		
		if(biY[i] != '0'){
			zeroY = 0;
		}
	}
	
	//se X é zero
	if(zeroX){
		return biY;
	}
	
	//se Y é zero
	if(zeroY){
		return biX;
	}
	
	return "";
	
}

int verifica_operacao(){
		int operacao, condicao = 0;
		
		while (condicao == 0){
			printf("\nQual operação você deseja fazer: \nPara SOMA (Z = X + Y) responda 1 \nPara SUBTRAÇÃO (Z = X - Y) responda 2\n\nSua resposta: ");
			scanf("%i", &operacao);
			system("cls");
			
			if(operacao == 1 || operacao == 2){ return operacao; }
			else{ printf("Valores diferentes de 1 ou 2 NÃO são válidos, tente novamente!\n");}
						
		}
	
}

int eh_numero_valido(char* str, int tam){
    int i = 0, condicao = 0;
    
    while(str[i] != '\0') {
        if(!isdigit(str[i])){
            return 0;
        }
        if(str[i] != '0' && str[i] != '1'){
    		return 0;
		}

        i++;
    }
    if (tam == 32){
			return 1;
	}
	
	return 0;
	  
}

char* inverter_binario(char* binary_str) {
    // converte a string binária para um número inteiro
    int num = strtol(binary_str, NULL, 2);
    
    // inverte todos os bits do número
    int inverted_num = ~num;
    
    // adiciona 1 ao resultado
    inverted_num += 1;
    
    // aloca memória para a string binária invertida
    char* inverted_binary_str = (char*) malloc(strlen(binary_str) + 1);
    
    // Converte o resultado de volta para uma string binária
    int i = 0;
    while (i < 32) {
        if ((inverted_num & (1 << i)) != 0) {
            inverted_binary_str[31-i] = '1';
        } else {
            inverted_binary_str[31-i] = '0';
        }
        i++;
    }
    inverted_binary_str[32] = '\0';
    
    return inverted_binary_str;
}

//extrai expoente
int32_t extrai_expoente(int32_t num){
	return (num & EXP_MASK)>>23;
}

//extrai sinal (bit mais significativo)
int32_t extrai_sinal(int32_t num){
	return num & BIT31_MASK;
}

//operacao de soma ou subtracao dos binarios
void operacao_bin(char* bin_x, char* bin_y){
	int32_t num_x = (int32_t) strtol(bin_x, NULL, 2);
	int32_t num_y = (int32_t) strtol(bin_y, NULL, 2);
    int32_t expoente_x = extrai_expoente(num_x);
	int32_t expoente_y = extrai_expoente(num_y);
	int32_t sinal_x = extrai_sinal(num_x);
    int32_t sinal_y = extrai_sinal(num_y);
	int32_t diferenca_expoentes = expoente_x - expoente_y;
	int32_t deslocamento_num, add_num;
	
	//desloca números para alinhar os expoentes, se forem diferentes
	if(diferenca_expoentes < 0){
		deslocamento_num = num_x >> (-diferenca_expoentes);
		add_num = num_y;
	}	
	else if(diferenca_expoentes > 0){
		deslocamento_num = num_y >> diferenca_expoentes;
		add_num = num_x;
	} 
	//não precisa deslocar (expoentes iguais)
	else {
		deslocamento_num = num_x;
		add_num = num_y;
	}
	
	//adicionar numero deslocado e o outro numero
	int sum = deslocamento_num + add_num;
	
	//checar se o resultado foi negativo e ajustar se necessário
	if(sum<0){
		sum = -sum;
		sum |= BIT31_MASK;
	} 
	else if(sinal_x != sinal_y){
		sum |= BIT31_MASK;
	}
	
	//determina sinal do resultado
    int32_t result_sign;
    if (sinal_x == sinal_y) {
 	   result_sign = sinal_x;
    } else if (sum >= 0) {
    	result_sign = sinal_x;
    } else {
    	result_sign = sinal_y;
    }
    
    // Determina expoente do resultado
    int32_t result_exp;
    if (diferenca_expoentes < 0) {
        result_exp = expoente_y;
    } else {
        result_exp = expoente_x;
    }
    
    int32_t mantissa = sum & MANTISSA_MASK;
    int32_t exponent = (result_exp << 23) & EXP_MASK;
    int32_t sign = result_sign & BIT31_MASK;
    int32_t final_result = sign | exponent | mantissa;

    
   	char* mantissa_str = malloc(24 * sizeof(char));
	char* exponent_str = malloc(9 * sizeof(char));
	char* sign_str = malloc(2 * sizeof(char));
	
	// Converte a mantissa para uma string binária de 23 bits
	itoa(final_result & MANTISSA_MASK, mantissa_str, 2);
	int len = strlen(mantissa_str);
	if (len < 23) {
    	int zeros = 23 - len;
    	memmove(mantissa_str + zeros, mantissa_str, len);
    	memset(mantissa_str, '0', zeros);
	}
	
	// Converte o expoente para uma string binária de 8 bits
	itoa((final_result >> 23) & 0xFF, exponent_str, 2);
	len = strlen(exponent_str);
	if (len < 8) {
    	int zeros = 8 - len;
    	memmove(exponent_str + zeros, exponent_str, len);
    	memset(exponent_str, '0', zeros);
	}
	
	// Converte o sinal para uma string binária de 1 bit
	itoa((final_result >> 31) & 0x1, sign_str, 2);
	
	
    // Concatena as três strings em uma única string de 32 bits
	char* resultado = malloc(33 * sizeof(char));
	strcpy(resultado, sign_str);
	strcat(resultado, exponent_str);
	strcat(resultado, mantissa_str);

    printf("Resultado: %s", resultado);
}


