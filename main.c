#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <pic.h>

#pragma config FOSC = INTOSC    // INTOSC=内部クロックを使用し、CLKINピン(RA5)はデジタルピンとする
#pragma config WDTE = OFF       // ウォッチドッグタイマを無効にする
#pragma config PWRTE = ON      // 電源ONから64ms後にプログラムを開始する
#pragma config MCLRE = OFF      // ピンはデジタル入力ピン(RA3)として機能する
#pragma config CP = OFF         // プログラムメモリーの保護はしない
#pragma config BOREN = ON       // 電源電圧がBORVより下がったらリセットを行う
#pragma config CLKOUTEN = OFF   // CLKOUTﾋﾟﾝは無効、RA4として使用する

#pragma config WRT = OFF        // 1KWのフラッシュメモリ自己書き込み保護しない
#pragma config STVREN = ON      // スタックがオーバフローやアンダーフローしたらリセットを行う
#pragma config BORV = LO        // リセット電圧(VBOR)のトリップポイントを低(1.9V)に設定する
#pragma config LPBOR = OFF      // 低消費電力ブラウンアウトリセットは無効とする
#pragma config LVP = ON         // 低電圧プログラミングを行う(RA3のI/Oピンは使用不可)

#define MHz 000000
#define _XTAL_FREQ 16MHz

static void Delay_ms(unsigned int DELAY_CNT) {
    for (unsigned int i = 0; i < DELAY_CNT; i++) {
        __delay_ms(1);
    }
}

static int abs(int t) {
    if(t < 0 ) return -t;
    if(t >= 0) return t;
}

void main(void) {
    
    OSCCON = 0b01111000; // internal clock 16MHz
    ANSELA = 0b00000000; // analog input all off
    TRISA = 0b00000000; // RA0,RA1,RA2,RA4,RA5 output / RA3 input only
    
    while(1) {
        RA0 = RA3;
    
    };
}

