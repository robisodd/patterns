//56b37c6f-792a-480f-b962-9a0db8c32aa4
#include "pebble.h"
#define UPDATE_MS 100 // Refresh rate in milliseconds

  
typedef struct Layer
{
  GRect bounds;
  GRect frame;
  bool clips : 1;
  bool hidden : 1;
  struct Layer *next_sibling;
  struct Layer *parent;
  struct Layer *first_child;
  struct Window *window;
  LayerUpdateProc update_proc;
} Layer;  

GRect   box = {{1, 32}, {32, 32}}; // {{x, y} {w, h}}
GPoint  vel = {1,1};
bool moving = false;
uint8_t offset = 0;

static Window *main_window;
static Layer *graphics_layer;
typedef struct pattern_struct {
  char* name;
  //uint8_t row[8];
  uint8_t* data;
} pattern_struct;

pattern_struct pattern[40];
uint8_t maxpat = 0, selected = 0;
bool inverting = false;  

//void add_pattern(uint8_t* data, char* name) {
void add_pattern(uint8_t* data, char* name) {
  pattern[maxpat].name = name;
  pattern[maxpat].data = data;
  maxpat++;
}



void init_patterns() {
/*
  //add_pattern((uint8_t []){  0,  0, 84,124,124, 56,146,124}, "Tulip");
  //add_pattern((uint8_t []){170, 85,170, 85,170, 85,170, 85}, "50% Gray");
  static uint8_t pat01[] = {  0,  0, 84,124,124, 56,146,124}; add_pattern(pat01, "Tulip");
  static uint8_t pat02[] = {170, 85,170, 85,170, 85,170, 85}; add_pattern(pat02, "50% Gray");
  static uint8_t pat03[] = {127, 65, 65, 65, 65, 65,127,  0}; add_pattern(pat03, "Boxes");
  static uint8_t pat04[] = {  0, 80,114, 32,  0,  5, 39,  2}; add_pattern(pat04, "Critters");
  static uint8_t pat05[] = { 32, 80,136, 80, 32,  0,  0,  0}; add_pattern(pat05, "Diamonds");
  static uint8_t pat06[] = {  2,  7,  7,  2, 32, 80, 80, 32}; add_pattern(pat06, "Paisley");
  static uint8_t pat07[] = {224,128,142,136,234, 10, 14,  0}; add_pattern(pat07, "Pattern");
  static uint8_t pat08[] = {130, 68, 40, 17, 40, 68,130,  1}; add_pattern(pat08, "Quilt");
  static uint8_t pat09[] = { 64,192,200,120,120, 72,  0,  0}; add_pattern(pat09, "Scottie");
  static uint8_t pat10[] = { 20, 12,200,121,158, 19, 48, 40}; add_pattern(pat10, "Spinner");
  static uint8_t pat11[] = {248,116, 34, 71,143, 23, 34,113}; add_pattern(pat11, "Thatches");
  static uint8_t pat12[] = {  0,  0,  0,  0,128,128,128,240}; add_pattern(pat12, "Waffle");
  static uint8_t pat13[] = {136, 84, 34, 69,136, 21, 34, 81}; add_pattern(pat13, "Weave");
  static uint8_t pat14[] = {187, 95,174, 93,186,117,234,245}; add_pattern(pat14, "Bricks");
  static uint8_t pat15[] = {170,125,198, 71,198,127,190, 85}; add_pattern(pat15, "Buttons");
  static uint8_t pat16[] = {120, 49, 19,135,225,200,140, 30}; add_pattern(pat16, "Cargo Net");
  static uint8_t pat17[] = { 82, 41,132, 66,148, 41, 66,132}; add_pattern(pat17, "Circuits");
  static uint8_t pat18[] = { 40, 68,146,171,214,108, 56, 16}; add_pattern(pat18, "Cobblestones");
  static uint8_t pat19[] = {130,  1,  1,  1,171, 85,170, 85}; add_pattern(pat19, "Colosseum");
  static uint8_t pat20[] = { 30,140,216,253,191, 27, 49,120}; add_pattern(pat20, "Daisies");
  static uint8_t pat21[] = { 62,  7,225,  7, 62,112,195,112}; add_pattern(pat21, "Dizzy");
  static uint8_t pat22[] = { 86, 89,166,154,101,149,106,169}; add_pattern(pat22, "Field Effect");
  static uint8_t pat23[] = {254,  2,250,138,186,162,190,128}; add_pattern(pat23, "Key");
  static uint8_t pat24[] = {239,239, 14,254,254,254,224,239}; add_pattern(pat24, "Live Wire");
  static uint8_t pat25[] = {240,240,240,240,170, 85,170, 85}; add_pattern(pat25, "Plaid");
  static uint8_t pat26[] = {215,147, 40,215, 40,147,213,215}; add_pattern(pat26, "Rounder");
  static uint8_t pat27[] = {225, 42, 37,146, 85,152, 62,247}; add_pattern(pat27, "Scales");
  static uint8_t pat28[] = {174, 77,239,255,  8, 77,174, 77}; add_pattern(pat28, "Stone");
  static uint8_t pat29[] = {248,116, 34, 71,143, 23, 34,113}; add_pattern(pat29, "Thatches");
  static uint8_t pat30[] = { 69,130,  1,  0,  1,130, 69,170}; add_pattern(pat30, "Tile");
  static uint8_t pat31[] = {135,  7,  6,  4,  0,247,231,199}; add_pattern(pat31, "Triangles");
  static uint8_t pat32[] = { 77,154,  8, 85,239,154, 77,154}; add_pattern(pat32, "Waffle's Revenge");
  static uint8_t pat33[] = {  0, 16, 68, 40,130, 40, 68, 16}; add_pattern(pat33, "Pat 1");
  static uint8_t pat34[] = {  4, 28,228, 66, 66, 39, 56, 32}; add_pattern(pat34, "Pat 2");
*/  
  static uint8_t pat01[] = {  0, 84,124,124, 56,146,124,  0}; add_pattern(pat01, "Tulip");
  static uint8_t pat02[] = {170, 85,170, 85,170, 85,170, 85}; add_pattern(pat02, "50% Gray");
  static uint8_t pat03[] = {127, 65, 65, 65, 65, 65,127,  0}; add_pattern(pat03, "Boxes");
  static uint8_t pat3a[] = {128,190,190,190,190,190,128,255}; add_pattern(pat3a, "Boxes 2");
  static uint8_t pat04[] = { 80,114, 32,  0,  5, 39,  2,  0}; add_pattern(pat04, "Critters");
  static uint8_t pat4a[] = {175,141,223,255,250,216,253,255}; add_pattern(pat4a, "Critters 2");
  static uint8_t pat05[] = {  0, 32, 80,136, 80, 32,  0,  0}; add_pattern(pat05, "Diamonds");
  static uint8_t pat5a[] = {255,223,175,119,175,223,255,255}; add_pattern(pat5a, "Diamonds 2");
  static uint8_t pat06[] = {  2,  7,  7,  2, 32, 80, 80, 32}; add_pattern(pat06, "Paisley");
  static uint8_t pat6a[] = {253,248,248,253,223,175,175,223}; add_pattern(pat6a, "Paisley 2");
  static uint8_t pat07[] = {254,125,187,215,238,215,187,125}; add_pattern(pat07, "Quilt");
  static uint8_t pat08[] = {255,191, 63, 55,135,135,183,255}; add_pattern(pat08, "Scottie");
  static uint8_t pat09[] = { 97,236,207,215,235,243, 55,134}; add_pattern(pat09, "Spinner");
  static uint8_t pat10[] = {  7,139,221,184,112,232,221,142}; add_pattern(pat10, "Thatches");
  static uint8_t pat12[] = {255,255,127,127,127, 15,255,255}; add_pattern(pat12, "Waffle");
  static uint8_t pat13[] = {119,171,221,186,119,234,221,174}; add_pattern(pat13, "Weave");
  static uint8_t pat14[] = {160, 81,162, 69,138, 21, 10, 68}; add_pattern(pat14, "Bricks");  // Oooh, good
  static uint8_t pat15[] = { 85,130, 57,184, 57,128, 65,170}; add_pattern(pat15, "Buttons");  // I like this one
  static uint8_t pat16[] = {135,206,236,120, 30, 55,115,225}; add_pattern(pat16, "Cargo Net");
  static uint8_t pat17[] = {173,214,123,189,107,214,189,123}; add_pattern(pat17, "Circuits");
  static uint8_t pat18[] = {239,215,187,109, 84, 41,147,199}; add_pattern(pat18, "Cobblestones"); // Cool too
  static uint8_t pat19[] = { 85,170,125,254,254,254, 84,170}; add_pattern(pat19, "Colosseum");
  static uint8_t pat20[] = {  2, 64,228,206,135,225,115, 39}; add_pattern(pat20, "Daisies");
  static uint8_t pat21[] = { 62,  7,225,  7, 62,112,195,112}; add_pattern(pat21, "Dizzy");
  static uint8_t pat22[] = { 86, 89,166,154,101,149,106,169}; add_pattern(pat22, "Field Effect");
  static uint8_t pat23[] = {253,  5,117, 69, 93, 64,127,  1}; add_pattern(pat23, "Key");
  static uint8_t pat24[] = {239,239, 14,254,254,254,224,239}; add_pattern(pat24, "Live Wire");
  static uint8_t pat25[] = {240,240,240,240,170, 85,170, 85}; add_pattern(pat25, "Plaid");
  static uint8_t pat26[] = { 40,215,108, 42, 40, 40,108,215}; add_pattern(pat26, "Rounder");       /// Pretty good, too
  static uint8_t pat27[] = {  8, 30,213,218,109,170,103,192}; add_pattern(pat27, "Scales");   // Good for dragon game
  static uint8_t pat28[] = {247,178, 81,178, 81,178, 16,  0}; add_pattern(pat28, "Stone");     // Good for Dungeon game
  static uint8_t pat29[] = {  7,139,221,184,112,232,221,142}; add_pattern(pat29, "Thatches");
  static uint8_t pat30[] = { 69,130,  1,  0,  1,130, 69,170}; add_pattern(pat30, "Tile");    // Good for diner game
  static uint8_t pat3b[] = {186,125,254,255,254,125,186, 85}; add_pattern(pat3b, "Tile 2");
  static uint8_t pat31[] = {247,231,199,135,  7,  6,  4,  0}; add_pattern(pat31, "Triangles");
  static uint8_t pat32[] = {154, 77,154, 77,154,  8, 85,239}; add_pattern(pat32, "Waffle's Revenge");  // Good
  static uint8_t pat33[] = {  0, 16, 68, 40,130, 40, 68, 16}; add_pattern(pat33, "Pat 1");
  static uint8_t pat34[] = {  4, 28,228, 66, 66, 39, 56, 32}; add_pattern(pat34, "Pat 2");
}

/*
void add_pattern(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h, char* name) {
  pattern[maxpat].name = name;
  pattern[maxpat].row[0] = a;
  pattern[maxpat].row[1] = b;
  pattern[maxpat].row[2] = c;
  pattern[maxpat].row[3] = d;
  pattern[maxpat].row[4] = e;
  pattern[maxpat].row[5] = f;
  pattern[maxpat].row[6] = g;
  pattern[maxpat].row[7] = h;
  maxpat++;
}

void init_patterns() {
  add_pattern(  0,  0, 84,124,124, 56,146,124, "Tulip");
  add_pattern(170, 85,170, 85,170, 85,170, 85, "50% Gray");
  add_pattern(127, 65, 65, 65, 65, 65,127,  0, "Boxes");
  add_pattern(  0, 80,114, 32,  0,  5, 39,  2, "Critters");
  add_pattern( 32, 80,136, 80, 32,  0,  0,  0, "Diamonds");
  add_pattern(  2,  7,  7,  2, 32, 80, 80, 32, "Paisley");
  add_pattern(224,128,142,136,234, 10, 14,  0, "Pattern");
  add_pattern(130, 68, 40, 17, 40, 68,130,  1, "Quilt");
  add_pattern( 64,192,200,120,120, 72,  0,  0, "Scottie");
  add_pattern( 20, 12,200,121,158, 19, 48, 40, "Spinner");
  add_pattern(248,116, 34, 71,143, 23, 34,113, "Thatches");
  add_pattern(  0,  0,  0,  0,128,128,128,240, "Waffle");
  add_pattern(136, 84, 34, 69,136, 21, 34, 81, "Weave");
  add_pattern(187, 95,174, 93,186,117,234,245, "Bricks");
  add_pattern(170,125,198, 71,198,127,190, 85, "Buttons");
  add_pattern(120, 49, 19,135,225,200,140, 30, "Cargo Net");
  add_pattern( 82, 41,132, 66,148, 41, 66,132, "Circuits");
  add_pattern( 40, 68,146,171,214,108, 56, 16, "Cobblestones");
  add_pattern(130,  1,  1,  1,171, 85,170, 85, "Colosseum");
  add_pattern( 30,140,216,253,191, 27, 49,120, "Daisies");
  add_pattern( 62,  7,225,  7, 62,112,195,112, "Dizzy");
  add_pattern( 86, 89,166,154,101,149,106,169, "Field Effect");
  add_pattern(254,  2,250,138,186,162,190,128, "Key");
  add_pattern(239,239, 14,254,254,254,224,239, "Live Wire");
  add_pattern(240,240,240,240,170, 85,170, 85, "Plaid");
  add_pattern(215,147, 40,215, 40,147,213,215, "Rounder");
  add_pattern(225, 42, 37,146, 85,152, 62,247, "Scales");
  add_pattern(174, 77,239,255,  8, 77,174, 77, "Stone");
  add_pattern(248,116, 34, 71,143, 23, 34,113, "Thatches");
  add_pattern( 69,130,  1,  0,  1,130, 69,170, "Tile");
  add_pattern(135,  7,  6,  4,  0,247,231,199, "Triangles");
  add_pattern( 77,154,  8, 85,239,154, 77,154, "Waffle's Revenge");
  add_pattern(  0, 16, 68, 40,130, 40, 68, 16, "Pat 1");
  add_pattern(  4, 28,228, 66, 66, 39, 56, 32, "Pat 2");

}
*/

// ------------------------------------------------------------------------ //
//  Timer Functions
// ------------------------------------------------------------------------ //
static void timer_callback(void *data) {
  //if(moving) {
    //if(box.origin.x + box.size.w + vel.x >143 || box.origin.x + vel.x < 0) vel.x *= -1;
    //if(box.origin.y + box.size.h + vel.y >167 || box.origin.y + vel.y < 0) vel.y *= -1;
  //  box.origin.x += vel.x;  box.origin.y += vel.y;
  //  box.size.w += vel.x;  box.size.h += vel.y;
  //}  // move box
  offset = (offset + 1) % 8;
  layer_mark_dirty(graphics_layer);
  app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback
}

// ------------------------------------------------------------------------ //
//  Button Functions
// ------------------------------------------------------------------------ //
// void     up_single_click_handler(ClickRecognizerRef recognizer, void *context) {box.origin.x-=1; box.origin.y-=1;}
// void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {box.size.w+=2; box.size.h+=2;}
// void   down_single_click_handler(ClickRecognizerRef recognizer, void *context) {box.origin.x+=20; box.origin.y+=20;}

void     up_single_click_handler(ClickRecognizerRef recognizer, void *context) {selected = (selected + 1) % maxpat;}// layer_mark_dirty(graphics_layer);}
//void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {moving = !moving; vel.x=rand()%3; vel.y=rand()%3;}
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {inverting = !inverting;}
void   down_single_click_handler(ClickRecognizerRef recognizer, void *context) {selected--; if(selected==255) selected = maxpat - 1;}// layer_mark_dirty(graphics_layer);}


void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

// ------------------------------------------------------------------------ //
//  Drawing Functions
// ------------------------------------------------------------------------ //
uint8_t* pattern_offset(uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert) {  // invert=0 or 255
  static uint8_t temp[8];
  uint8_t mask = ((x_offset%8)+8)%8;  // convert [-128 to 127] to [0 to 7]
  for(uint8_t i=0; i<8; i++)
    temp[i] = ((data[((y_offset%8)+8+i)%8] << mask) + (data[((y_offset%8)+8+i)%8] >> (8-mask))) ^ invert; // shift 8bit segment x&y offset, invert using XOR
  //if(invert) for(uint8_t i=0; i<8; i++) temp[i] = ~temp[i];
  return temp;
}

void fill_window(GContext *ctx, uint8_t *data) {
  uint8_t* aaaa = (uint8_t*)*(uint32_t*)ctx;
//  for(uint16_t y=0, yaddr=0; y<168; y++, yaddr+=20)
//    for(uint16_t x=0; x<19; x++)
//      aaaa[yaddr+x] = data[y%8];
  //uint8_t inv=0;  if(inverting) inv=1;
  
  for(uint16_t y=0; y<168; y++)
    for(uint16_t x=0; x<144; x++)
      (aaaa)[(y*144)+x] = ( ( ((int)inverting) ^ ((data[y%8]>>((x+offset)%8))&1)) * 0b11001100);
}

void fill_window_plus(GContext *ctx, uint8_t *data, uint8_t x_offset, uint8_t y_offset, bool invert) {
  uint8_t* aaaa = (uint8_t*)*((uint32_t*)ctx);
  if(invert)
    for(uint16_t y=0, yaddr=0; y<168; y++, yaddr+=20)
      for(uint16_t x=0; x<19; x++)
        aaaa[yaddr+x] = ~((data[(y+y_offset)%8] << x_offset) + (data[(y+y_offset)%8] >> (8-x_offset)));
  else
    for(uint16_t y=0, yaddr=0; y<168; y++, yaddr+=20)
      for(uint16_t x=0; x<19; x++)
        aaaa[yaddr+x] = (data[(y+y_offset)%8] << x_offset) + (data[(y+y_offset)%8] >> (8-x_offset));
}

void fill_rect(GContext *ctx, GRect rect, uint8_t *data, bool invert, uint8_t x_offset, uint8_t y_offset) {
  uint8_t* aaaa = (uint8_t*)*((uint32_t*)ctx);
  uint8_t segment[8];
  GPoint addr;
  int16_t y_init, rows, y;  // Y memory address init, new height of box, incremental variable
  
  if(rect.origin.x+rect.size.w<0 || rect.origin.y+rect.size.h<0 || rect.origin.x>143 || rect.origin.y>167) return;  // Out of bounds, nothing to display
  
  //Step 1: Rotate segments to match offset
  uint8_t mask = (x_offset+rect.origin.x)%8;
  if(invert) for(uint8_t i=0; i<8; i++) segment[i] = ~((data[i] << mask) + (data[i] >> (8-mask))); // shift 8bit segment x&y offset, then invert bits
  else       for(uint8_t i=0; i<8; i++) segment[i] =  ((data[i] << mask) + (data[i] >> (8-mask))); // shift 8bit segment x&y offset
  
  addr.x = rect.origin.x >> 3;       // init X memory address

  if(rect.origin.y < 0){
    y_offset -= rect.origin.y % 8;
    y_init    = 0;
    rows      = rect.size.h + rect.origin.y;
  } else {
    y_init    = rect.origin.y;
    rows      = rect.size.h;
  }
  if(y_init + rows>168) rows = 168 - y_init;
  y_init *= 20;
  
  mask = 255 << (rect.origin.x%8);
  if(addr.x >= 0 && addr.x < 19)
    for (y=0, addr.y=y_init; y<rows; y++, addr.y+=20)
      aaaa[addr.y + addr.x] = (segment[(y_offset+y)%8] & mask) + (aaaa[addr.y + addr.x] & ~mask); // fill left-side of row
    
  for (addr.x++; addr.x<((rect.origin.x+rect.size.w)>>3); addr.x++)
    if(addr.x >= 0 && addr.x < 19)
      for (y=0, addr.y=y_init; y<rows; y++, addr.y+=20)
        aaaa[addr.y + addr.x] = segment[(y_offset+y)%8];           // fill middle of row
  
  mask = 255 << ((rect.origin.x+rect.size.w)%8);
  if(addr.x >= 0 && addr.x < 19)
    for (y=0, addr.y=y_init; y<rows; y++, addr.y+=20)
      aaaa[addr.y + addr.x] = (aaaa[addr.y + addr.x] & mask) + (segment[(y_offset+y)%8] & ~mask); // fill right-side of row
}


/*
void fill_rect(GContext *ctx, GRect rect, uint8_t *data, bool invert, uint8_t x_offset, uint8_t y_offset) {
  uint8_t segment[8];
  GPoint addr;

  GRect boxx;// = {{0,0},{0,0}};
  
  if(rect.origin.x+rect.size.w<0 || rect.origin.y+rect.size.h<0 || rect.origin.x>143 || rect.origin.y>167) return;  // Out of bounds, nothing to display
  if(rect.origin.x<0) {
    boxx.origin.x = 0;  // unnecessary
    boxx.size.w = rect.size.w + rect.origin.x;
    x_offset -= (rect.origin.x % 8);
  } else {
    boxx.origin.x = rect.origin.x;
    boxx.size.w = rect.size.w;
  }
  if(boxx.origin.x + boxx.size.w>143) boxx.size.w = 143 - boxx.origin.x;
      
  if(rect.origin.y < 0){
    boxx.origin.y = 0;  // unnecessary
    boxx.size.h = rect.size.h + rect.origin.y;
    y_offset -= (rect.origin.y % 8);
  } else {
    boxx.origin.y = rect.origin.y;
    boxx.size.h = rect.size.h;
  }
  if(boxx.origin.y + boxx.size.h>167) boxx.size.h = 167 - boxx.origin.y;
  
  
  
  uint8_t mask = (x_offset+boxx.origin.x)%8;
  if(invert) for(uint8_t i=0; i<8; i++) segment[i] = ~((data[i] << mask) + (data[i] >> (8-mask))); // shift 8bit segment x&y offset, then invert bits
  else       for(uint8_t i=0; i<8; i++) segment[i] =  ((data[i] << mask) + (data[i] >> (8-mask))); // shift 8bit segment x&y offset
  
  uint8_t lmask = 255 << (boxx.origin.x%8);               // mask for the left side
  uint8_t rmask = 255 << ((boxx.origin.x+boxx.size.w)%8); // mask for the right side
  addr.y = boxx.origin.y*20;
  for (int16_t y=0; y<boxx.size.h; y++, y_offset=(y_offset+1)%8, addr.y+=20) {
    addr.x = boxx.origin.x>>3;       // init X memory address
    if(boxx.size.w>((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (segment[y_offset] & lmask) + (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & ~lmask); // fill left-side of rect
    for (addr.x++; addr.x<((boxx.origin.x+boxx.size.w)>>3); addr.x++) ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = segment[y_offset]; // fill middle of rect
    ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & rmask) + (segment[y_offset] & ~rmask); // fill right-side of rect
  }
}


      


void fill_rect(GContext *ctx, GRect rect, uint8_t *data, bool invert, uint8_t x_offset, uint8_t y_offset) {
  uint8_t segment[8];
  GPoint addr;

  uint8_t l_mask = 255 << (rect.origin.x%8);               // mask for the left side
  uint8_t r_mask = 255 << ((rect.origin.x+rect.size.w)%8); // mask for the right side
  uint8_t s_mask = (x_offset+rect.origin.x)%8;             // mask for x_offset shifting
  if(invert) for(uint8_t i=0; i<8; i++) segment[i] = ~((data[i] << s_mask) + (data[i] >> (8-s_mask))); // shift 8bit segment x&y offset, then invert bits
  else       for(uint8_t i=0; i<8; i++) segment[i] =  ((data[i] << s_mask) + (data[i] >> (8-s_mask))); // shift 8bit segment x&y offset
  
  addr.y = rect.origin.y*20;
  for (int16_t y=0; y<rect.size.h; y++, y_offset=(y_offset+1)%8, addr.y+=20) {
    if(addr.y>=0 && addr.y<168*20) {   // if y row is on the screen
      addr.x = rect.origin.x>>3;       // init X memory address
      if(addr.x >= 0 && addr.x < 19) ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (segment[y_offset] & l_mask) + (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & ~l_mask); // fill left-side of row
      for (addr.x++; addr.x<((rect.origin.x+rect.size.w)>>3); addr.x++) if(addr.x >= 0 && addr.x < 19) ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = segment[y_offset];           // fill middle of row
      if(addr.x >= 0 && addr.x < 19) ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & r_mask) + (segment[y_offset] & ~r_mask); // fill right-side of row
    }
  }
}


      // fill left-side of rect
      addr.x = rect.origin.x>>3;
      if(addr.x >= 0 && addr.x < 19)
        ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (segment & lmask) + (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & ~lmask);
    
      // fill middle of rect
      for (addr.x=((rect.origin.x)>>3)+1; addr.x<((rect.origin.x+rect.size.w)>>3); addr.x++)
        if(addr.x >= 0 && addr.x < 19)
          ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = segment;

      
      // fill right-side of rect
      if(addr.x >= 0 && addr.x < 19)
       ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & rmask) + (segment & ~rmask);
*/
      


/*  

void fill_rect(GContext *ctx, GRect rect, uint8_t *data, bool invert, uint8_t x_offset, uint8_t y_offset) {
  //graphics_fill_rect(GContext *ctx, GRect rect, uint16_t corner_radius, GCornerMask corner_mask)
  uint8_t segment;
  GPoint addr, pos;
  //GPoint addr = {0, 0};  GPoint pos = {0, 0};
  
  for (int16_t y=0; y<rect.size.h; y++) {
     pos.y = rect.origin.y + y;
    if(pos.y>=0 && pos.y<168) {
      addr.y = pos.y *20;  // translate y position to memory address
      pos.x = rect.origin.x+rect.size.w;
      
      // shift 8bit segment x&y offset
      segment = (data[(y+y_offset)%8] << ((x_offset+rect.origin.x)%8)) + (data[(y+y_offset)%8] >> (8-((x_offset+rect.origin.x)%8)));
      
      // invert segment
      if(invert) segment=~segment;

      // fill left-side of rect
      addr.x = rect.origin.x>>3;
      if(addr.x >= 0 && addr.x < 19)
        ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (segment & (255 << (rect.origin.x%8))) + (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & (255 >> (8-(rect.origin.x%8))));
    
      // fill middle of rect
      for (addr.x=((rect.origin.x)>>3)+1; addr.x<((rect.origin.x+rect.size.w)>>3); addr.x++)
        if(addr.x >= 0 && addr.x < 19)
          ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = segment;

      // fill right-side of rect
      if(addr.x >= 0 && addr.x < 19)
       ((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] = (((uint8_t*)(((GBitmap*)ctx)->addr))[addr.y + addr.x] & (255 << (pos.x%8))) + (segment & (255 >> (8-(pos.x%8))));
      
      
      
    }
  }
}


void fill_window(GContext *ctx, uint8_t patID) {
  for (uint16_t y=0; y<168; y++)
    for (uint16_t x=0; x<19; x++)
      ((uint8_t*)(((GBitmap*)ctx)->addr))[y*20+x] = pattern[patID].row[y%8];
}

void fill_window_plus(GContext *ctx, uint8_t patID, uint8_t x_offset, uint8_t y_offset) {
  for (uint16_t y=0; y<168; y++)
    for (uint16_t x=0; x<19; x++)
      ((uint8_t*)(((GBitmap*)ctx)->addr))[y*20+x] = (pattern[patID].row[(y+y_offset)%8] << x_offset) + (pattern[patID].row[(y+y_offset)%8] >> (8-x_offset));  // x&y offset shifted
    //pattern[patID].row[y%8];
}


void fill_rect(GContext *ctx, uint8_t *data, GRect rect, uint8_t x_offset, uint8_t y_offset) {
  uint8_t segment=0;
  //uint8_t currentbits;
  uint32_t address;
  GPoint addr = {0, 0};
  GPoint pos = {0, 0};
  y_offset = y_offset % 8;
  x_offset = x_offset % 8;
  for (int16_t y=0; y<rect.size.h; y++) {
     pos.y=rect.origin.y+y;
    if(pos.y>=0 && pos.y<168) {
      segment = (pattern[selected].row[(y+y_offset)%8] << ((x_offset+rect.origin.x)%8)) + (pattern[selected].row[(y+y_offset)%8] >> (8-((x_offset+rect.origin.x)%8)));  // 8bit segment, x&y offset shifted
    
      addr.y = pos.y *20;
      
      // left-side filling
      pos.x = rect.origin.x;
      addr.x = pos.x>>3;
      if(addr.x >= 0 && addr.x < 19) {
        address = addr.y + addr.x;
        //currentbits = ((uint8_t*)(((GBitmap*)ctx)->addr))[address];  // get 8 bit segment
        //currentbits = currentbits & (255 >> (8-(pos.x%8)));  // blacken right side of line
        //((uint8_t*)(((GBitmap*)ctx)->addr))[address] = (segment & (255 << (pos.x%8))) + currentbits;
        
        ((uint8_t*)(((GBitmap*)ctx)->addr))[address] = (segment & (255 << (pos.x%8))) + (((uint8_t*)(((GBitmap*)ctx)->addr))[address] & (255 >> (8-(pos.x%8))));
      }
    
      // middle filling
      if(rect.size.w>15)  // try to get rid of this line and test
      //for (int16_t x=1; x<=(rect.size.w-7)>>3); x++) {
      for (int16_t x=((rect.origin.x)>>3)+1; x<((rect.origin.x+rect.size.w)>>3); x++) {
        //pos.x = rect.origin.x+rect.size.w;
        addr.x = x;//(rect.origin.x + x)>>3;
        if(addr.x >= 0 && addr.x < 19) {
          address=addr.y + addr.x;
          ((uint8_t*)(((GBitmap*)ctx)->addr))[address] = segment;
        }
      }
       
       //for (int16_t x=(rect.origin.x>>3)+1; x<((rect.origin.x + rect.size.w)>>3); x++) ((uint8_t*)(((GBitmap*)ctx)->addr))[ypos + x] = what;

      
      // right-side
      pos.x = rect.origin.x+rect.size.w;
      addr.x = pos.x>>3;
      if(addr.x >= 0 && addr.x < 19) {
        address=addr.y + addr.x;
        //currentbits = ((uint8_t*)(((GBitmap*)ctx)->addr))[address];  // get 8 bit segment
        //currentbits = currentbits & (255 << (pos.x%8));  // blacken left side of line
        //((uint8_t*)(((GBitmap*)ctx)->addr))[address] = currentbits;// + (segment & (255 >> (8-(pos.x%8))));
        
        
       ((uint8_t*)(((GBitmap*)ctx)->addr))[address] = (((uint8_t*)(((GBitmap*)ctx)->addr))[address] & (255 << (pos.x%8))) + (segment & (255 >> (8-(pos.x%8))));
        
      }
      
      
    }
  }
}
*/
static void graphics_layer_update(Layer *me, GContext *ctx) {
  GRect textframe;
  static char text[100];  //Buffer to hold text

  fill_window(ctx, (uint8_t []){170, 85,170, 85,170, 85,170, 85});    // Fill window with 50% Gray
  fill_window(ctx, pattern[selected].data);
  //fill_window_plus(ctx, pattern[selected].data, inverting, 0, 0);
  
  //fill_rect(ctx, box, pattern[selected].data, false, offset, offset);
  //fill_rect(ctx, box, pattern[selected].data, false, 0, 0);
  //graphics_context_set_stroke_color(ctx, GColorWhite); graphics_draw_rect(ctx, box);                //White Rectangle Border
  
  textframe = GRect(11, 10, 120, 20);  // Text Box Position and Size: x, y, w, h
  snprintf(text, sizeof(text), "%d: %s", selected, pattern[selected].name);  // What text to draw
  graphics_context_set_fill_color(ctx, GColorBlack); graphics_fill_rect(ctx, textframe, 0, GCornerNone);  //Black Filled Rectangle
  graphics_context_set_stroke_color(ctx, GColorWhite); graphics_draw_rect(ctx, textframe);                //White Rectangle Border
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);  //Write Text

  textframe = GRect(11, 40, 120, 120);  // Text Box Position and Size: x, y, w, h
  graphics_context_set_fill_color(ctx, GColorBlack); graphics_fill_rect(ctx, textframe, 0, GCornerNone);  //Black Filled Rectangle
  graphics_context_set_stroke_color(ctx, GColorWhite); graphics_draw_rect(ctx, textframe);                //White Rectangle Border
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  //snprintf(text, sizeof(text), "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", pattern[selected].data[0]);  // What text to draw
  textframe.origin.y-=2;
  for(uint8_t i=0; i<8; i++){
    snprintf(text, sizeof(text), "%03u  %03u", pattern[selected].data[i], (~pattern[selected].data[i])&255);  // What text to draw
    graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);  //Write Text    
    textframe.origin.y+=12;
  }
}
  
// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  graphics_layer = layer_create(layer_get_frame(window_layer));
  layer_set_update_proc(graphics_layer, graphics_layer_update);
  layer_add_child(window_layer, graphics_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(graphics_layer);
}

static void init(void) {
  pattern[selected].name = "empty";  // Crashes without this
  // Set up and push main window
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_click_config_provider(main_window, click_config_provider);
  window_set_background_color(main_window, GColorBlack);
  window_set_fullscreen(main_window, true);
  
  window_stack_push(main_window, true /* Animated */); // Display window.  Callback will be called once layer is dirtied then written
  
  init_patterns();
  
  app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback
}
  
static void deinit(void) {
  accel_data_service_unsubscribe();
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

// ------------------------------------------------------------------------ //
//  Notes
// ------------------------------------------------------------------------ //
/*
#define FONT_KEY_FONT_FALLBACK "RESOURCE_ID_FONT_FALLBACK"
#define FONT_KEY_GOTHIC_14 "RESOURCE_ID_GOTHIC_14"
#define FONT_KEY_GOTHIC_14_BOLD "RESOURCE_ID_GOTHIC_14_BOLD"
#define FONT_KEY_GOTHIC_18 "RESOURCE_ID_GOTHIC_18"
#define FONT_KEY_GOTHIC_18_BOLD "RESOURCE_ID_GOTHIC_18_BOLD"
#define FONT_KEY_GOTHIC_24 "RESOURCE_ID_GOTHIC_24"
#define FONT_KEY_GOTHIC_24_BOLD "RESOURCE_ID_GOTHIC_24_BOLD"
#define FONT_KEY_GOTHIC_28 "RESOURCE_ID_GOTHIC_28"
#define FONT_KEY_GOTHIC_28_BOLD "RESOURCE_ID_GOTHIC_28_BOLD"
#define FONT_KEY_BITHAM_30_BLACK "RESOURCE_ID_BITHAM_30_BLACK"
#define FONT_KEY_BITHAM_42_BOLD "RESOURCE_ID_BITHAM_42_BOLD"
#define FONT_KEY_BITHAM_42_LIGHT "RESOURCE_ID_BITHAM_42_LIGHT"
#define FONT_KEY_BITHAM_42_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_42_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_34_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_LIGHT_SUBSET "RESOURCE_ID_BITHAM_34_LIGHT_SUBSET"
#define FONT_KEY_BITHAM_18_LIGHT_SUBSET "RESOURCE_ID_BITHAM_18_LIGHT_SUBSET"
#define FONT_KEY_ROBOTO_CONDENSED_21 "RESOURCE_ID_ROBOTO_CONDENSED_21"
#define FONT_KEY_ROBOTO_BOLD_SUBSET_49 "RESOURCE_ID_ROBOTO_BOLD_SUBSET_49"
#define FONT_KEY_DROID_SERIF_28_BOLD "RESOURCE_ID_DROID_SERIF_28_BOLD"





*/
