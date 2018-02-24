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
    
    unsigned int temp;
    unsigned int addat;
    int width = 0;
    int max_width = 0;
    long int panicTimer = 0;
    int i = 0;

 //   OSCCON  = 0b01110000; // 内部クロックを32MHzとする
 //   OSCCON  = 0b01111010; // 内部クロックを16MHzとする
    OSCCON = 0b01110010; // 内部クロックは8MHzとする PLLEN = ONにて4倍速動作指定済み
    ANSELA = 0b0000001; // アナログはAN0を使用し、残りをすべてデジタルI/Oに割当
    TRISA  = 0b00001001; // RA3は入力専用, アナログはAN0を使用
    ADCON0 = 0b00000001; // アナログ変換情報設定(RA0ポートのAN0から読込む)
    __delay_us(5);
    ADCON1 = 0b10010000; // 読取値は右寄せ、A/D変換クロックはFOSC/8、VDDをリファレンスとする
//    ADCON1 = 0b10100000; // 読取値は右寄せ、A/D変換クロックはFOSC/32、VDDをリファレンスとする
    Delay_ms(1); // アナログ変換情報が設定されるまでとりあえず待つ (*1:20→5)

    RA2 = 0; RA1 = 0;
    RA5 = 0; RA4 = 0;
    
    Delay_ms(100);
    
    while(1) {
        
        if((width > 0)&&(RA3 == 0)) {
            max_width = (width - 250)*3;// 0
            if(max_width<0) max_width = 0;
            width = 0;
            GO = 1; // start conversion
            while(nDONE);  // wait conversion
            addat = ADRESH ;        // PICは読取った値をADRESHとADRESLのレジスターにセットする
            addat = ( addat << 8 ) | ADRESL ;  // 10ビットの分解能力です
        };
        
        if((width == 0)&&(RA3 == 1)) {
            while(RA3) width++;
        };
                   
        if((5 <= max_width)&&(max_width < 973)) { // 
            
            if(abs(addat - max_width) > 60) {//30
                panicTimer = 80000;// 80000
            };
           
            panicTimer--;if (panicTimer < 0) panicTimer = 0;
            
            if((abs(addat - max_width) > 30)&&(panicTimer > 0)) {
                
                if(addat > max_width) {
                    RA5 = 0; RA4 = 1;
                } else {
                    RA5 = 1; RA4 = 0;
                };
                
            } else {RA5 = 1; RA4 = 1;}; // lock // if(abs(ADRES - max_width) > 25)

        } else {RA5 = 0; RA4 = 0;}; // free // if((5 <= max_width)&&(max_width < 973))
        
    };  //  while(1)

    
} // main
