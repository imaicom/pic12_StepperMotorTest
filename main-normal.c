#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <pic.h>

// CONFIG1
#pragma config FOSC = INTOSC    // INTOSC= 内部クロックを使用し、CLKINピン(RA5)はデジタルピンとする
#pragma config WDTE = OFF       // ウォッチドッグタイマを無効にする
#pragma config PWRTE = ON       // ON = 電源ONから64ms後にプログラムを開始する
#pragma config MCLRE = OFF      // ピンはデジタル入力ピン(RA3)として機能する
#pragma config CP = OFF         // プログラムメモリーの保護はしない
#pragma config CPD = OFF        // データメモリーの保護はしない
#pragma config BOREN = ON       // 電源電圧降下常時監視機能は有効とする
#pragma config CLKOUTEN = OFF   // CLKOUTピンは無効、RA4ピンとして使用する
#pragma config IESO = ON        // 内部から外部クロックへの切替えで起動を行う
#pragma config FCMEN = OFF      // 外部クロックの監視はしない

// CONFIG2
#pragma config WRT = OFF        // 2KWのフラッシュメモリ自己書き込み保護ビットを保護しない
#pragma config PLLEN = ON       // 4xPLLを動作させる(クロックを32MHzで動作させる場合に使用)
#pragma config STVREN = ON      // スタックがオーバフローやアンダーフローしたらリセットを行う
#pragma config BORV = LO        // リセット電圧(VBOR)のトリップポイントを低(1.9V)に設定する
#pragma config LVP = OFF         // 低電圧プログラミングしない

#define MHz 000000
#define _XTAL_FREQ 32MHz

// PICKIT3 Setting
//
// Program Options
//   Erase All Before Program [*]
//   Program Speed [2(fastest)]
//   Enable Low Voltage Programming [ ]
//   Programming Method [Apply Vdd before Vpp]
//
// Power
//   Power target circuit from PICkit3 [*]
//   Voltage Level [4.5]

static void Delay_ms(unsigned int DELAY_CNT) {
    for (unsigned int i = 0; i < DELAY_CNT; i++) {
        __delay_ms(1);
    }
}

void main(void) {
    
    int i = 0;

    OSCCON =     0b01110010; // 内部クロックは8MHzとする PLLEN = ONにて4倍速動作指定済み
    ANSELA =     0b00000000; // アナログ入力は停止、すべてデジタルI/Oに割当
    TRISA  =     0b00000100; // RA2は入力、RA3は入力専用
    OPTION_REG = 0b00000010; // プルアップ　オン
    WPUA =       0b00001100; // RA3とRA2をプルアップ指定

    
    while(1) {
        
        if(i==0) {PORTA = 0b00110000;} // RA5=1;RA4=1;RA1=0;RA0=0;
        if(i==1) {PORTA = 0b00010010;} // RA5=0;RA4=1;RA1=1;RA0=0;
        if(i==2) {PORTA = 0b00000011;} // RA5=0;RA4=0;RA1=1;RA0=1;
        if(i==3) {PORTA = 0b00100001;} // RA5=1;RA4=0;RA1=0;RA0=1;
        
        if(!RA2) {while(!RA2)Delay_ms(10); i++; if(i>3)i=0; };
        if(!RA3) {while(!RA3)Delay_ms(10); i--; if(i<0)i=3; };
        
    };  //  while(1)

} // main
