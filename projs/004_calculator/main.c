#include<stdio.h>


typedef enum {
	ADD = 1,
	SUBTRACT,
	DIVIDE,
	MULTIPLY
} Operation;

int divide(float num, float denum, float *res);
float multiply(float num1, float num2);
float add(float num1, float num2);
float subtract(float bignum, float smallnum);

void get_operation(Operation *calc, char msg[]);
void get_number(float *num, char msg[]) ;
void show_msg(char msg[]) ;

int main() {
	int op_input = 0;
	Operation calc;
	float num1, num2;
	float result;
	char msg[] =  "Please Enter your operation:\n 1 Add\n 2 Subtract\n 3 Divide\n 4 Multiply\n";
	get_operation(&calc, msg);
	

	get_number(&num1, "Please enter the 1st number\n");
	get_number(&num2, "Please enter the 2st number\n");

	switch(calc) {
		case 1 : {
			result = add(num1, num2);
			break;
		}
		case 2: {
			result = subtract(num1, num2);
			break;
		}
		case 3 : {
			int err;
			err = divide(num1, num2, &result);
			if(err) {
				fprintf(stderr, "ERROR: Divisible by 0 error\n");
				return 1;
			}
			break;
		}
		case 4: {
			result = multiply(num1, num2);
			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unexoected math case encountered\n");
			printf("Unexpected case encountered \n"); return 1;
		}
	}

	while(1) {
		printf("The last answer is %.2f\n", result);
		get_operation(&calc, msg);
		get_number(&num2, "Please enter the next number\n");

		switch(calc) {
		case 1:{
			result = add(result, num2);
			break;
			}
		
		case 2: {
			result = subtract(result, num2);
				break;
			}

		case 3:{
			int err;
			float temp ;
			err = divide(result, num2, &temp);
			if(err) {
				fprintf(stderr, "ERROR: Divisible by 0 error\n");
				return 1;
			}
			result = temp;
			break;
			}

		case 4: {
				result = multiply(result, num2);
				break;
			}

		default: {
				fprintf(stderr, "ERROR: Unexoected math case encountered\n");
				printf("Unexpected case encountered \n"); return 1;
			}

		}
	}
	
	

	return 0;
}

void get_operation(Operation *calc, char msg[]){
	int op_input= 0;
	
	show_msg(msg);;

	do{
		//int ret = scanf("%d", &op_input);
		if ((scanf("%d", &op_input)) != 1){
			while(getchar() != '\n');
			op_input = 0;
			printf("Invalid input\n");
			show_msg(msg);
			
			continue; 
		}
	} while(op_input < 1 || op_input > 4);

	*calc = (Operation) op_input;
}

void show_msg(char msg[]) {
	int i = 0;
	while(msg[i] != '\0'){
		putchar(msg[i]);
		i++;
	}
}

void get_number(float *num, char msg[]) {
	int err = 0, i = 0;
	float local;
	while(msg[i] != '\0'){
		putchar(msg[i]); 
		i++;
	}
	// printf("Please enter the number\n");
	do {
		err = scanf("%f", &local);
		if (err != 1) {
			while (getchar() != '\n');
			{
				printf("Invalid input. Please enter a number\n");
			}
			
		}
	} while (err != 1);
	*num = local;
}

float add(float num1, float num2) {
	return num1 + num2;
}

float subtract(float bignum, float smallnum){
	return bignum - smallnum;
}

int divide(float num, float denum, float *res){
	if(denum == 0){
		return 1;
	}
	*res = num/denum;
	return 0;
}

float multiply(float num1, float num2){
	return num1 * num2;
}
