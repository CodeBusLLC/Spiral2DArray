
#define DBG_PRINTF(_str_, ...) //printf(_str_,__VA_ARGS__)
#include <stdio.h>  // for printf
#include <string.h> // for memset
#include <stdlib.h> // for exit

#define ARR_SIZE 8 // declares the size of the array
#define NUM_MODES 4

struct Populate2DAsSpiral
{
    typedef enum { eRight, eDown, eLeft, eUp } Direction_t; // direction of movement
    typedef enum { eOutCounterClockwise, eOutClockwise, eInClockwise, eInCounterClockwise } SpiralMode_t; // type of spiral
    
    void    create(int aSize, int *aArray, SpiralMode_t aMode);

    void    initialize();
    void    move();
    void    directionChange();
    void    errorAndExit(const char* apStr);
    
    int             max;            // runaway catch
    SpiralMode_t    mode;           // how to populate
    int             size;           // size of array
    Direction_t     direction;      // direction in movement (right,left,up,down)
    int             row;            // current row
    int             col;            // current column
    int             dirChange;      // count til next direction change
    static const char*  title[NUM_MODES];
};

static Populate2DAsSpiral p2daS; // singleton

const char*  Populate2DAsSpiral::title[NUM_MODES]={ "Out Counter Clockwise", "Out Clockwise", "In Clockwise", "In Counter Clockwise" };

void Populate2DAsSpiral::errorAndExit(const char* apStr)
{
    printf("%s\n", apStr);
    exit(-1);
}

void Populate2DAsSpiral::initialize()  //-------------------------------
{
  max = 0;
  direction = eRight;
  dirChange = size - 1;
  row = 0;
  col = 0;
  
  switch(mode)
  {
  case eOutCounterClockwise:
    dirChange = 1;
    row = size / 2;
    col = size / 2;
    if( (size & 1) == 0 )
    {
        col--;
    }
    break;
    
  case eOutClockwise:
    dirChange = 1;
    row = size / 2;
    col = size / 2;
    if( (size & 1) == 0 )
    {
        row--;
        col--;
    }
    break;
    
  case eInCounterClockwise:
    row = size - 1;
    col = size - 1;
    direction = eLeft;
    break;
    
  default:
    break;
  }
} // end of "initialize()"  _______________________________

void Populate2DAsSpiral::move()
{
    switch (direction)
	{
	case eRight:
	  {
	    DBG_PRINTF("right in row=%d col=%d\n", row, col);
	    col++;
	    DBG_PRINTF("right out row=%d col=%d\n", row, col);
	    break;
	  }
	case eDown:
	  {
	    DBG_PRINTF("down in row=%d col=%d\n", row, col);
	    row++;
	    DBG_PRINTF("down out row=%d col=%d\n", row, col);
	    break;
	  }
	case eLeft:
	  {
	    DBG_PRINTF("left in row=%d col=%d\n", row, col);
	    col--;
	    DBG_PRINTF("left out row=%d col=%d\n", row, col);
	    break;
	  }
	case eUp:
	  {
	    DBG_PRINTF("up in row=%d col=%d\n", row, col);
	    row--;
	    DBG_PRINTF("up out row=%d col=%d\n", row, col);
	    break;
	  }
	default:
	    errorAndExit("internal error: invalid direction");
	}
} // end of "increment()"  __________________________

void Populate2DAsSpiral::directionChange()
{
  switch( mode )
  {
  case eOutCounterClockwise:
	  switch (direction)
      {
      case eRight:
          direction = eUp;
          break;
          
      case eDown:
          direction = eRight;
          dirChange++;
          break;
          
      case eLeft:
          direction = eDown;
          break;
          
      case eUp:
          direction = eLeft;
          dirChange++;
          break;
          
      default:
  	      errorAndExit("internal error: invalid direction");
      }
      break;
	    
  case eOutClockwise:
	  switch (direction)
	  {
	  case eRight:
	      direction = eDown;
	      break;
	      
	  case eDown:
	      direction = eLeft;
	      dirChange++;
	      break;
	      
	  case eLeft:
	      direction = eUp;
	      break;
	      
	  case eUp:
	      direction = eRight;
	      dirChange++;
	      break;
	      
      default:
  	      errorAndExit("internal error: invalid direction");
	  }
	  break;
	    
  case eInClockwise:
	  switch (direction)
	  {
	  case eRight:
	      direction = eDown;
	      if( row != 0)
	      {
	          dirChange--;    	      
	      }
	      break;
	      
	  case eDown:
	      direction = eLeft;
	      break;
	      
	  case eLeft:
	      direction = eUp;
	      dirChange--;    	      
	      break;
	      
	  case eUp:
	      direction = eRight;
	      break;

      default:
  	      errorAndExit("internal error: invalid direction");
	    }
	    break;
	    
  case eInCounterClockwise:
	  switch (direction)
	  {
	  case eRight:
	      direction = eDown;
          dirChange--;    	      
	      break;
	      
	  case eDown:
	      direction = eLeft;
	      break;
	      
	  case eLeft:
	      direction = eUp;
	      if( row != (size - 1))
	      {
	        dirChange--;
	      }
	      break;
	      
	  case eUp:
	      direction = eRight;
	      break;
	      
      default:
  	      errorAndExit("internal error: invalid direction");
	  }
	  break;
	    
  default:
      errorAndExit("Invalid mode");
  } // end "switch(mode)"
} // end of "directionChange()"  ________________________________________

void Populate2DAsSpiral::create(int aSize, int *aArray, SpiralMode_t aMode)
{
    mode = aMode;
    size = aSize;
      
    initialize();
    
    printf("%s\n", title[mode]);
    
    int s_ = 0;
    int index_ = 0;

    aArray[(row * size) + col] = s_++; // place first item
    
    while (s_ < (size * size))
    {
        move();
    
        aArray[(row * size) + col] = s_++; // set specified cell
        
        index_++;
        if ( (dirChange - index_) == 0)
    	{
          index_ = 0;
          directionChange();
    	}
    	
        if( ++max > (size * size) )
        {
          printf("runaway!\n");
          break;
        }
    } // end of "while(s < sizeOfArray)"
} // end of "create()"  ________________________________________

void print2DArray(int aSize, const int* aArray)
{
    for (int r_ = 0; r_ < ARR_SIZE; r_++)
    {
        for (int c_ = 0; c_ < ARR_SIZE; c_++)
    	{
    	  printf ("%02d ", aArray[(r_ * aSize) + c_]);
    	}
        printf ("\n");
    }
    printf("\n");
}

int main ()
{
  int arr[ARR_SIZE][ARR_SIZE];

  for( Populate2DAsSpiral::SpiralMode_t m_ = Populate2DAsSpiral::eOutCounterClockwise; 
       m_ <= Populate2DAsSpiral::eInCounterClockwise;
       m_ = (Populate2DAsSpiral::SpiralMode_t)(m_ + 1) 
     )  
  {
      memset( &arr, 0, ARR_SIZE * ARR_SIZE);
      p2daS.create(ARR_SIZE, (int*)&arr, m_);
      print2DArray(ARR_SIZE, (int*)&arr);
  }
  
  return 0;
}
