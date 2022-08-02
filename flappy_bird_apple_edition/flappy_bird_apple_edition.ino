   //ВНИМАНИЕ, ВОЗМОЖЕН ГОВНОКОД!!!

#include <Wire.h>
#include <SFE_MicroOLED.h>

#define res 255
#define addr 0

MicroOLED oled(res, addr);

#define applePin 13 //пин к которому подключено яблоко. На esp32 это 32, 33, 27, 14, 12, 13, 4, 2, 15

int OLED_width = oled.getLCDWidth(); //ширина дисплея
int OLED_height = oled.getLCDHeight(); //высота дисплея

int r = 2; //радиус круга(птицы)

int bird_elevation = OLED_height - r - 1; //высота птицы

int tube_width = 3; //ширина труб

int top_tube_heigh; //высота верхней трубы

int tube_x = OLED_width; //координата X труб

int down_tube_y; //координата Y нижней трубы

int score = 0; //счет

bool game = true;

int tube_distance = 12; //расстояние между трубами

int record = 0; //рекорд


void setup() {
  top_tube_heigh = random(3, OLED_height - 8 - 3 - tube_distance + 1); //устанавливаем случайную высоту верхней трубы

  oled.begin();

  Wire.setClock(800000L);//устанавливаем максимальную скорость

  oled.clear(ALL);
  oled.clear(PAGE);//очищаем дисплей
}

void loop() {
  if (game) {
    down_tube_y = top_tube_heigh + tube_distance + 8; //координата Y нижней трубы

    oled.clear(PAGE);//очищаем дисплей


    //рисуем птицу
    oled.circleFill(16, bird_elevation, r);


    //если яблоко нажато, поднимать птицу выше
    //иначе опускать птицу ниже
    if (touchRead(13) < 40) {
      if (bird_elevation > (8 + r + 2)) {
        bird_elevation -= 1;
      }
    } else {
      if (bird_elevation < (OLED_height - r - 1)) {
        bird_elevation += 1;
      }
    }


    oled.setCursor(0, 0);

    oled.print("score:");
    oled.print(score);//выводим счет на дисплей

    oled.line(0, 8, OLED_width, 8);


    //рисуем трубы
    oled.rectFill(tube_x, 8, tube_width, top_tube_heigh);
    oled.rectFill(tube_x - 1, top_tube_heigh + 8 - 1, tube_width + 2, 2);

    oled.rectFill(tube_x, down_tube_y, tube_width, OLED_height - 8);
    oled.rectFill(tube_x - 1, down_tube_y, tube_width + 2, 2);

    //двигаем трубы влево
    tube_x -= 1;

    //если трубы достигли левого края, переместить их к правому краю и добавляем к счету игрока 1
    if (tube_x == 0) {
      top_tube_heigh = random(3, OLED_height - 8 - 3 - tube_distance + 1);
      tube_x = OLED_width;
      score += 1;
    }

    //если птица касается трубы завершить игру
    if (tube_x <= (16 + r) && tube_x >= (16 - r + 1 - tube_width)) {
      if (bird_elevation <= (8 + top_tube_heigh + r) || bird_elevation >= (8 + top_tube_heigh + tube_distance - r)) {
        game = false;
      }
    }

    oled.display();//отображаем все на дсплей
  } else {
    //устанавливаем новый рекорд
    if(score > record){
      record = score;
    }
    
    //если игра завершена
    oled.clear(PAGE);//очищаем дисплей

    oled.setCursor(0, 14);
    oled.setFontType(0); //шрифт 5x7

    oled.print("score:");
    oled.print(score);//выводим счет на дисплей

    oled.print("\n");

    oled.print("record:");
    oled.print(record);//выводим рекорд на дисплей

    oled.display();

    delay(3000);

    score = 0;//обнуляем счет
    tube_x = OLED_width;
    bird_elevation = OLED_height - r - 1;
    game = true;//начинаем игру
  }
}
