  const byte pin_Left_Ear      =  0;    //D3          // левое ухо  //
  const byte pin_Right_Ear     =  2;    //D4          // правое ухо //

  const byte pin_Smile_Yellow  =  5;    //D1  
  const byte pin_Smile_Orange  =  4;    //D2
  const byte pin_Smile_Red     =  14;   //D5

  const byte pin_Left_Eye      = 12;    //D6
  const byte pin_Right_Eye     = 13;    //D7

  namespace game
  {
    enum
    {
      ZERO   = 0,
      ONE    = 1,
      TWO    = 2,
      THREE  = 3,
      FOUR   = 4,
      FIVE   = 5,
      SIX    = 6,
    } stage;

    int stage_TWO_counter;
    int stage_THREE_counter;
    int stage_FOUR_sublevels;
    int stage_FIVE_sublevels;
    unsigned long stage_SIX_countdown = 0;
  }  
  
void setup()
{
  Serial.begin(115200);

  pinMode(pin_Left_Ear,  INPUT_PULLUP);          // определяем пины //
  pinMode(pin_Right_Ear, INPUT_PULLUP);          // ↑↑↑ //

  pinMode(pin_Smile_Yellow, OUTPUT);
  pinMode(pin_Smile_Orange, OUTPUT);
  pinMode(pin_Smile_Red,    OUTPUT);
  pinMode(pin_Left_Eye,     OUTPUT);
  pinMode(pin_Right_Eye,    OUTPUT);

  stage_ZERO();
}

void loop()
{
  stage_select();
}

void stage_select()
{
  switch(game::stage)
  {
    case game::ZERO:
    {
      stage_ZERO();
      break;
    }
    case game::ONE:
    {
      stage_ONE();
      break;
    }
    case game::TWO:
    {
      stage_TWO();
      break;
    }
    case game::THREE:
    {
      stage_THREE();
      break;
    }
    case game::FOUR:
    {
      stage_FOUR();
      break;
    }    
    case game::FIVE:
    {
      stage_FIVE();
      break;
    } 
    case game::SIX:
    {
      stage_SIX();
      break;
    } 
  }
}
void stage_ZERO()
{
  digitalWrite(pin_Smile_Yellow, HIGH);
  digitalWrite(pin_Smile_Orange, HIGH);
  digitalWrite(pin_Smile_Red,    HIGH);

  digitalWrite(pin_Left_Eye, HIGH);
  digitalWrite(pin_Right_Eye, HIGH);
  game::stage = game::ONE;
}

void stage_ONE()
{
  static unsigned long millis_stage_ONE_timer;
  static bool timer_start;

  if(digitalRead(pin_Left_Ear) == 0)
  {
    digitalWrite(pin_Left_Eye, LOW);
    if(timer_start == false)
    {
      millis_stage_ONE_timer = millis();
      timer_start = true;
    }
  }
  else
  {
    digitalWrite(pin_Left_Eye, HIGH);
  }

  if(digitalRead(pin_Right_Ear) == 0)
  {
    digitalWrite(pin_Right_Eye, LOW);
    if(timer_start == false)
    {
      millis_stage_ONE_timer = millis();
      timer_start = true;
    }
  }
  else
  {
    digitalWrite(pin_Right_Eye, HIGH);
  }

  if(digitalRead(pin_Left_Ear) == 1 && digitalRead(pin_Right_Ear) == 1)
  {
    timer_start = false;
    millis_stage_ONE_timer = 0;
  }

  if(digitalRead(pin_Left_Ear) == 0 && digitalRead(pin_Right_Ear) == 0)
  {
    digitalWrite(pin_Left_Eye,  LOW);       // если вначале было нажато правое ухо, чтобы оба светодиода потухли прежде чем перейти на следующий уровень
    digitalWrite(pin_Right_Eye, LOW);

    if(millis() - millis_stage_ONE_timer < 40)
    {
      Serial.println("COMPLETE lvl_1");
      game::stage_TWO_counter = 0;
      game::stage = game::TWO;
    }
  }  
}

void stage_TWO()
{
  if(digitalRead(pin_Left_Ear) == 0 && digitalRead(pin_Right_Ear) == 0)
  {
    if(game::stage_TWO_counter < 100)
    {
      game::stage_TWO_counter++;
      delay(10);
    }

    else if(game::stage_TWO_counter == 100)
    {
      game::stage_TWO_counter++;
      digitalWrite(pin_Smile_Yellow, LOW);
    }    

    else if(game::stage_TWO_counter < 200)
    {
      game::stage_TWO_counter++;
      delay(10);
    }

    else if(game::stage_TWO_counter == 200)
    {
      game::stage_TWO_counter++;
      digitalWrite(pin_Smile_Orange, LOW);
    }

    else if(game::stage_TWO_counter < 300)
    {
      game::stage_TWO_counter++;
      delay(10);
    }

    else if(game::stage_TWO_counter == 300)
    {
      digitalWrite(pin_Smile_Red, LOW);

      game::stage = game::THREE;
      game::stage_THREE_counter = 0;
    }  
  }

  else
  {
    game::stage = game::ZERO;
  }
}

void stage_THREE()
{
  if(game::stage_THREE_counter < 20)
  {
    game::stage_THREE_counter++;
    delay(10);
    if(digitalRead(pin_Left_Ear) == 1 && digitalRead(pin_Right_Ear) == 1)
    {
      Serial.println("COMPLETE lvl_2");
      game::stage = game::FOUR;
      game::stage_FOUR_sublevels = 0;
    }
  }

  else if(game::stage_THREE_counter == 20)
  {
    game::stage = game::ZERO;
  }
}

void stage_FOUR()
{
  static unsigned long yellow_led_millis_start;
  static unsigned long yellow_led_millis_end;
  static unsigned long yellow_led_millis_delay;
  static unsigned long orange_led_millis_start;
  static unsigned long orange_led_millis_end;
  static unsigned long orange_led_millis_delay;
  static unsigned long red_led_millis_start;
  static unsigned long red_led_millis_end;
  static unsigned long red_led_millis_delay;

  static unsigned long millis_stage_FOUR_timer;
  static bool left_eye_state;

  if(millis() - millis_stage_FOUR_timer > 400)
  {
    if(left_eye_state == false)
    {
      digitalWrite(pin_Left_Eye,  LOW);
      left_eye_state = true;
    }
    else
    {
      digitalWrite(pin_Left_Eye,  HIGH);
      left_eye_state = false;
    }
    millis_stage_FOUR_timer = millis();
  }

  static byte local_level;

  if(digitalRead(pin_Right_Ear) == 0)
  {
    game::stage_FOUR_sublevels = 0;
    Serial.println("stage 4 reset");
  }

  static unsigned long yellow_led_timer;
  static bool yellow_led_state;
  if(yellow_led_millis_delay != 0)
  {
    if(millis() - yellow_led_timer > yellow_led_millis_delay)
    {
      if(yellow_led_state == false)
      {
        digitalWrite(pin_Smile_Yellow,  LOW);
        yellow_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Yellow,  HIGH);
        yellow_led_state = false;
      }
      yellow_led_timer = millis();      
    }
  }

  static unsigned long orange_led_timer;
  static bool orange_led_state;
  if(orange_led_millis_delay != 0)
  {
    if(millis() - orange_led_timer > orange_led_millis_delay)
    {
      if(orange_led_state == false)
      {
        digitalWrite(pin_Smile_Orange,  LOW);
        orange_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Orange,  HIGH);
        orange_led_state = false;
      }
      orange_led_timer = millis();      
    }
  }

  static unsigned long red_led_timer;
  static bool red_led_state;
  if(red_led_millis_delay != 0)
  {
    if(millis() - red_led_timer > red_led_millis_delay)
    {
      if(red_led_state == false)
      {
        digitalWrite(pin_Smile_Red,  LOW);
        red_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Red,  HIGH);
        red_led_state = false;
      }
      red_led_timer = millis();      
    }
  }

  switch(game::stage_FOUR_sublevels)
  {
    case 0:
    {
      yellow_led_millis_start = 0;
      yellow_led_millis_end   = 0;
      yellow_led_millis_delay = 0;
      orange_led_millis_start = 0;
      orange_led_millis_end   = 0;
      orange_led_millis_delay = 0;
      red_led_millis_start    = 0;
      red_led_millis_end      = 0;
      red_led_millis_delay    = 0;

      digitalWrite(pin_Smile_Yellow,  LOW);
      digitalWrite(pin_Smile_Orange,  LOW);
      digitalWrite(pin_Smile_Red,     LOW);

      game::stage_FOUR_sublevels = 1;
      break;
    }

    case 1:
    {
      if(digitalRead(pin_Left_Ear) == 0)
      {
        digitalWrite(pin_Smile_Yellow,  HIGH);
        yellow_led_millis_start = millis();
        game::stage_FOUR_sublevels = 2;
      }
      delay(1);
      break;
    }

    case 2:
    {
      if(digitalRead(pin_Left_Ear) == 1)
      {
        digitalWrite(pin_Smile_Yellow,  LOW);
        yellow_led_millis_end = millis();
        yellow_led_millis_delay = yellow_led_millis_end - yellow_led_millis_start;
        game::stage_FOUR_sublevels = 3;
      }
      delay(1);
      break;
    }

    case 3:
    {
      if(digitalRead(pin_Left_Ear) == 0)
      {
        digitalWrite(pin_Smile_Orange,  HIGH);
        orange_led_millis_start = millis();
        game::stage_FOUR_sublevels = 4;
      }
      delay(1);
      break;
    }

    case 4:
    {
      if(digitalRead(pin_Left_Ear) == 1)
      {
        digitalWrite(pin_Smile_Orange,  LOW);
        orange_led_millis_end = millis();
        orange_led_millis_delay = orange_led_millis_end - orange_led_millis_start;
        game::stage_FOUR_sublevels = 5;
      }
      delay(1);
      break;
    }

    case 5:
    {
      if(digitalRead(pin_Left_Ear) == 0)
      {
        digitalWrite(pin_Smile_Red,  HIGH);
        red_led_millis_start = millis();
        game::stage_FOUR_sublevels = 6;
      }
      delay(1);
      break;
    }

    case 6:
    {
      if(digitalRead(pin_Left_Ear) == 1)
      {
        digitalWrite(pin_Smile_Red,  LOW);
        red_led_millis_end = millis();
        red_led_millis_delay = red_led_millis_end - red_led_millis_start;
        game::stage_FOUR_sublevels = 7;
      }
      delay(1);
      break;
    }

    case 7:
    {
      /*
      Serial.print("yellow: ");
      Serial.println(yellow_led_millis_delay);
      Serial.print("orange: ");
      Serial.println(orange_led_millis_delay);
      Serial.print("red: ");
      Serial.println(red_led_millis_delay);
      delay (200);
      */

      if(yellow_led_millis_delay > 340 && yellow_led_millis_delay < 460)
      {
        if(orange_led_millis_delay > 340 && orange_led_millis_delay < 460)
        {
          if(red_led_millis_delay > 340 && red_led_millis_delay < 460)
          {
            Serial.println("COMPLETE lvl_4");
            game::stage_FIVE_sublevels = 0;
            game::stage = game::FIVE;
          }
        }
      }
      break;
    }  
  }
}

void stage_FIVE()
{
  static unsigned long yellow_led_millis_start;
  static unsigned long yellow_led_millis_end;
  static unsigned long yellow_led_millis_delay;
  static unsigned long orange_led_millis_start;
  static unsigned long orange_led_millis_end;
  static unsigned long orange_led_millis_delay;
  static unsigned long red_led_millis_start;
  static unsigned long red_led_millis_end;
  static unsigned long red_led_millis_delay;

  static unsigned long millis_stage_FIVE_timer;
  static bool right_eye_state;

  if(millis() - millis_stage_FIVE_timer > 700)
  {
    if(right_eye_state == false)
    {
      digitalWrite(pin_Right_Eye,  LOW);
      right_eye_state = true;
    }
    else
    {
      digitalWrite(pin_Right_Eye,  HIGH);
      right_eye_state = false;
    }
    millis_stage_FIVE_timer = millis();
  }

  static byte local_level;

  if(digitalRead(pin_Left_Ear) == 0)
  {
    game::stage_FIVE_sublevels = 0;
    Serial.println("stage 5 reset");
  }

  static unsigned long yellow_led_timer;
  static bool yellow_led_state;
  if(yellow_led_millis_delay != 0)
  {
    if(millis() - yellow_led_timer > yellow_led_millis_delay)
    {
      if(yellow_led_state == false)
      {
        digitalWrite(pin_Smile_Yellow,  LOW);
        yellow_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Yellow,  HIGH);
        yellow_led_state = false;
      }
      yellow_led_timer = millis();      
    }
  }

  static unsigned long orange_led_timer;
  static bool orange_led_state;
  if(orange_led_millis_delay != 0)
  {
    if(millis() - orange_led_timer > orange_led_millis_delay)
    {
      if(orange_led_state == false)
      {
        digitalWrite(pin_Smile_Orange,  LOW);
        orange_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Orange,  HIGH);
        orange_led_state = false;
      }
      orange_led_timer = millis();      
    }
  }

  static unsigned long red_led_timer;
  static bool red_led_state;
  if(red_led_millis_delay != 0)
  {
    if(millis() - red_led_timer > red_led_millis_delay)
    {
      if(red_led_state == false)
      {
        digitalWrite(pin_Smile_Red,  LOW);
        red_led_state = true;
      }
      else
      {
        digitalWrite(pin_Smile_Red,  HIGH);
        red_led_state = false;
      }
      red_led_timer = millis();      
    }
  }

  switch(game::stage_FIVE_sublevels)
  {
    case 0:
    {
      yellow_led_millis_start = 0;
      yellow_led_millis_end   = 0;
      yellow_led_millis_delay = 0;
      orange_led_millis_start = 0;
      orange_led_millis_end   = 0;
      orange_led_millis_delay = 0;
      red_led_millis_start    = 0;
      red_led_millis_end      = 0;
      red_led_millis_delay    = 0;

      digitalWrite(pin_Smile_Yellow,  LOW);
      digitalWrite(pin_Smile_Orange,  LOW);
      digitalWrite(pin_Smile_Red,     LOW);

      digitalWrite(pin_Left_Eye,      LOW);

      game::stage_FIVE_sublevels = 1;
      break;
    }

    case 1:
    {
      if(digitalRead(pin_Right_Ear) == 0)
      {
        digitalWrite(pin_Smile_Yellow,  HIGH);
        yellow_led_millis_start = millis();
        game::stage_FIVE_sublevels = 2;
      }
      delay(1);
      break;
    }

    case 2:
    {
      if(digitalRead(pin_Right_Ear) == 1)
      {
        digitalWrite(pin_Smile_Yellow,  LOW);
        yellow_led_millis_end = millis();
        yellow_led_millis_delay = yellow_led_millis_end - yellow_led_millis_start;
        game::stage_FIVE_sublevels = 3;
      }
      delay(1);
      break;
    }

    case 3:
    {
      if(digitalRead(pin_Right_Ear) == 0)
      {
        digitalWrite(pin_Smile_Orange,  HIGH);
        orange_led_millis_start = millis();
        game::stage_FIVE_sublevels = 4;
      }
      delay(1);
      break;
    }

    case 4:
    {
      if(digitalRead(pin_Right_Ear) == 1)
      {
        digitalWrite(pin_Smile_Orange,  LOW);
        orange_led_millis_end = millis();
        orange_led_millis_delay = orange_led_millis_end - orange_led_millis_start;
        game::stage_FIVE_sublevels = 5;
      }
      delay(1);
      break;
    }

    case 5:
    {
      if(digitalRead(pin_Right_Ear) == 0)
      {
        digitalWrite(pin_Smile_Red,  HIGH);
        red_led_millis_start = millis();
        game::stage_FIVE_sublevels = 6;
      }
      delay(1);
      break;
    }

    case 6:
    {
      if(digitalRead(pin_Right_Ear) == 1)
      {
        digitalWrite(pin_Smile_Red,  LOW);
        red_led_millis_end = millis();
        red_led_millis_delay = red_led_millis_end - red_led_millis_start;
        game::stage_FIVE_sublevels = 7;
      }
      delay(1);
      break;
    }

    case 7:
    {
      /*
      Serial.print("yellow: ");
      Serial.println(yellow_led_millis_delay);
      Serial.print("orange: ");
      Serial.println(orange_led_millis_delay);
      Serial.print("red: ");
      Serial.println(red_led_millis_delay);
      delay (200);
      */

      if(yellow_led_millis_delay > 620 && yellow_led_millis_delay < 780)
      {
        if(orange_led_millis_delay > 620 && orange_led_millis_delay < 780)
        {
          if(red_led_millis_delay > 620 && red_led_millis_delay < 780)
          {
            Serial.println("COMPLETE lvl_5");
            
            digitalWrite(pin_Smile_Yellow,  LOW);
            digitalWrite(pin_Smile_Orange,  LOW);
            digitalWrite(pin_Smile_Red,     LOW);

            digitalWrite(pin_Left_Eye,      LOW);
            digitalWrite(pin_Right_Eye,     LOW);
            
            game::stage_SIX_countdown = millis();
            game::stage = game::SIX;
          }
        }
      }
      break;
    }  
  }
}

void stage_SIX()
{


  if(millis() - game::stage_SIX_countdown > 1000 * 30)
  {
    game::stage = game::ZERO;
  }
}