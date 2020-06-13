/**
 * Implement addHex().
 * @author Matthew Hyeun
 */

#include "hex.h"
#include <string.h>
#include <stdlib.h>
/**
 * Converts a hexademical digit to its integer value.
 * @param ascii character representing a hexadecimal digit
 * @return value of hexadecimal digit or -1 on error
 */
int8_t hToI(char h) {
	if(!c_assert((h>='0' && h<='9') || (h>='A' && h<='F'))) return -1;
	int8_t val;
	if(h>='0' && h<='9') val = h - '0';
	else if(h>='A' && h<='F') val = h - 'A' + 10;
	return val;
}

/**
 * Converts an integer to its hexademical character representation.
 * @param integer in range [0,15]
 * @return ascii code of hexademical digit or 0 on error
 */
char iToH(uint8_t i) {
	if(!c_assert(i>=0 && i<=15)) return 0;
	if(i < 10) return '0' + i;
	else return 'A' + i - 10;
}

// Print Error
bool printZTooSmall(int k){
  c_assert(k > 0);
  return false;
}

/**
 * Add hex strings x and y, placing result in z.
 * @param x C string presenting hexademical number
 * @param y C string presenting hexademical number
 * @param z C string with enough storage for sum of x and y
 * @return true on success, false on failure
 */
bool addHex(char z[], char x[], char y[]) {
  if((strlen(x) == 0) && (strlen(y) == 0)){
    c_assert(!(strlen(x) == 0) && (strlen(y) == 0));
    return false;
  }

  int i = strlen(x)-1;
  int j = strlen(y)-1;
  int k = strlen(z)-1;
  int carry = 0;

  while (i > -1 || j > -1) {
    if(k < 0) return printZTooSmall(k);

    int currentX = 0;
    int currentY = 0;
    if(j < 0) {
      if(!c_assert((x[i]>='0' && x[i]<='9') || (x[i]>='A' && x[i]<='F'))) return false;
      currentX = hToI(x[i]);
    } else if(i < 0) {
      if(!c_assert((y[j]>='0' && y[j]<='9') || (y[j]>='A' && y[j]<='F'))) return false;
      currentY = hToI(y[j]);
    } else {
      if(!c_assert((x[i]>='0' && x[i]<='9') || (x[i]>='A' && x[i]<='F'))) return false;
      if(!c_assert((y[j]>='0' && y[j]<='9') || (y[j]>='A' && y[j]<='F'))) return false;
      currentX = hToI(x[i]);
      currentY = hToI(y[j]);
    }

    int sum = currentX + currentY + carry; // sum corresponding digits

    if(sum >= 16) {
      carry = sum / 16;
      sum = sum % 16;
    } else {
      carry = 0;
    }

    char sumToHex = iToH(sum);
    z[k] = sumToHex;
    i--;
    j--;
    k--;
  }

  if(carry != 0){
    if(k < 0) return printZTooSmall(k);
    z[k] = iToH(carry);
    carry = 0;
  }

  if(z[0] == 'x') {
    int xIndex = 0;
    while(z[xIndex] == 'x'){
      z[xIndex] = '0';
      xIndex++;
    }
  }
  return true;
}