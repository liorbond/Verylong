#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>

typedef char* verylong;

verylong insert_verylong() {
    int size = 0;
    // For \0
    verylong out = (verylong)malloc(sizeof(char) * size + 1);
    char c = getch();

    while(c != '\r' && c != '\n') {
        if(c < '0' || c > '9') {
            printf("Encountered bad character: %c or %d\n", c, c);
            return NULL;
        }

        out = (verylong)realloc(out, sizeof(char) * (size + 1) + 1);
        out[size] = c;
        size++;

        c = getch();
    }

    out[size] = '\0';
}

verylong add_verylong(verylong vl1, verylong vl2) {

    if(vl1 == NULL || vl2 == NULL) {
        return NULL;
    }

    int vl1_size = strlen(vl1);
    int vl2_size = strlen(vl2);
    int max_size = vl1_size > vl2_size? vl1_size : vl2_size;
    int min_size = vl1_size < vl2_size? vl1_size : vl2_size;
    verylong max_vl_ptr = vl1_size > vl2_size? vl1 : vl2;
    int carry = 0;
    int local_sum = 0;

    verylong out = (verylong)malloc(sizeof(char) * max_size + 1);

    for(int i = 0; i < min_size; i++) {
        local_sum = (vl1[vl1_size - i - 1] - '0') + (vl2[vl2_size - i - 1] - '0') + carry;
        if(local_sum >= 10) {
            carry = 1;
            local_sum -= 10;
        } else {
            carry = 0;
        }

        out[max_size - i - 1] = local_sum + '0';
    }

    for(int i = 0; i < max_size - min_size; i++) {
        out[i] = max_vl_ptr[i];
    }

    int current_pos = max_size - min_size - 1;

    if(min_size != max_size) {
       while(carry == 1 && current_pos >= 0) {
           local_sum = (out[current_pos] - '0') + 1;
           if(local_sum == 10) {
               local_sum = 0;
               carry = 1;
           } else {
               carry = 0;
           }

           out[current_pos] = local_sum + '0';
           current_pos--;
       }
    }

    // There is a ending carry
    if(carry == 1) {
        out = (verylong)realloc(out, sizeof(char) * (max_size + 1) + 1);
        for(int i = max_size; i >= 0; i--) {
            out[i + 1] = out[i];
        }

        out[0] = '1';
        max_size++;
    }

    out[max_size] = '\0';
    return out;
}

verylong mul_verylong(verylong vl1, verylong vl2) {

    if(vl1 == NULL || vl2 == NULL) {
        return NULL;
    }

    int vl1_size = strlen(vl1);
    int vl2_size = strlen(vl2);
    int max_size = vl1_size > vl2_size? vl1_size : vl2_size;
    int min_size = vl1_size < vl2_size? vl1_size : vl2_size;
    verylong max_vl_ptr = vl1_size > vl2_size? vl1 : vl2;
    verylong min_vl_ptr = vl2_size < vl1_size? vl2 : vl1;
    int local_sum;
    
    verylong temp_row;
    int temp_row_size;

    verylong total_sum;

    int temp_sum_size = 1;
    verylong temp_sum = (verylong)malloc(temp_sum_size * sizeof(char) + 1);


    /** 
     * 1234 * 12 - max_size 4 min size 2
     *  1234
     * *  12
     *  _____
     *  2468
     * 12340
     * 
     * Conclusion - amount of rows is min
     */ 

    verylong rows[min_size];
    for(int i = 0; i < min_size; i++) {
        rows[i] = (verylong)malloc(sizeof(char) * 2);
        rows[i][0] = '0';
        rows[i][1] = '\0';
    }

    for(int i = min_size - 1; i >= 0; i--) {
        for(int j = max_size - 1; j >= 0; j--) {
            // calculate sum without multipliers
            local_sum = (min_vl_ptr[i] - '0') * (max_vl_ptr[j] - '0');
            // create verylong
            if(local_sum >= 10) {
                temp_sum_size++;
                temp_sum = (verylong)realloc(temp_sum, 2 * sizeof(char) + 1);
                temp_sum[0] = local_sum / 10 + '0';
                temp_sum[1] = local_sum % 10 + '0';
            } else {
                temp_sum = (verylong)realloc(temp_sum, 1 * sizeof(char) + 1);
                temp_sum[0] = local_sum % 10 + '0';
            }

            

            // muliply
            for(int w = 0; w < max_size - 1 - j; w++) {
                temp_sum_size++;
                temp_sum = (verylong)realloc(temp_sum, temp_sum_size * sizeof(char) + 1);
                temp_sum[temp_sum_size - 1] = '0';
            }

            // add \0
            temp_sum[temp_sum_size] = '\0';

            // add temp_sum to row
            temp_row = rows[i];

            rows[i] = add_verylong(rows[i], temp_sum);
            free(temp_row);

            temp_sum_size = 1;
        }
        
        temp_row_size = strlen(rows[i]) + min_size - 1 - i;
        rows[i] = (verylong)realloc(rows[i], temp_row_size + 1);

        // multiply
        for(int m = 0; m < min_size - 1 - i; m++) {
            rows[i][temp_row_size - m - 1] = '0';
        }

        // add \0
        rows[i][temp_row_size] = '\0';
    }

    total_sum = rows[0];
    for(int i = 1; i < min_size; i++) {
        temp_row = total_sum;
        total_sum = add_verylong(total_sum, rows[i]);
        free(temp_row);
    }

    return total_sum;
}

void main() {   
    verylong vl1 = (verylong)malloc(4);
    verylong vl2 = (verylong)malloc(2);

    vl1[0] = '1';
    vl1[1] = '2';
    vl1[2] = '3';
    vl1[3] = '4';

    vl2[0] = '1';
    vl2[1] = '2';
 
    verylong pre = mul_verylong(vl1, vl1);
    pre = mul_verylong(pre, pre);
    pre = mul_verylong(pre, pre);
    pre = mul_verylong(pre, pre);
    pre = mul_verylong(pre, pre);
    pre = mul_verylong(pre, pre);
    verylong vl_end_2 = mul_verylong(pre, pre);
    verylong vl_print = mul_verylong(vl_end_2, vl_end_2);
    printf("sum of %s and %s is %s\n", vl_end_2, vl_end_2, add_verylong(vl_end_2, vl_end_2));
    printf("mul of %s and %s is %s len is %ld\n", vl_end_2, vl_end_2, vl_print, strlen(vl_print));
} 
