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

unsigned int adconv(void)
{
     unsigned int temp;

     GO_nDONE = 1 ;         // PICにアナログ値読取り開始を指示
     while(GO_nDONE) ;      // PICが読取り完了するまで待つ
     temp = ADRESH ;        // PICは読取った値をADRESHとADRESLのレジスターにセットする
     temp = ( temp << 8 ) | ADRESL ;  // 10ビットの分解能力です

     return temp ;
}

void main(void) {
    
    OSCCON = 0b01111000; // internal clock 16MHz
    ANSELA = 0b00000100; // analog 2 only on
    TRISA  = 0b00000100; // RA0,RA1,RA4,RA5 output / RA2,RA3 input
    PORTA  = 0b00000000; // PORTA ALL LOW OUTPUT
    ADCON0 = 0b00001001; // AN2 READ
    __delay_us(5);
    ADCON1 = 0b10010000; // 値は右寄せ , 001:FOSC/8 , VDD reference
    
//    PWM4CON = 0b11000000 ; // PWM4機能を使用する(output is active-high)
//    PWM4DCH = 0 ;          // デューティ値は０で初期化
//    PWM4DCL = 0 ;
//    PWM3CON = 0b11000000 ; // PWM3機能を使用する(output is active-high)
//    PWM3DCH = 0 ;          // デューティ値は０で初期化
//    PWM3DCL = 0 ;
//    T2CON   = 0b00000010 ; // TMR2プリスケーラ値を１６倍に設定
//    TMR2    = 0 ;          // タイマー２カウンターを初期化
//    PR2     = 124 ;        // PWMの周期を設定（1000Hzで設定）
//    TMR2ON  = 1 ;          // TMR2(PWM)スタート

    while(1) {
        RA0 = RA3;
    
    };
}

