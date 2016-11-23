//clang 3.7.0
// Author : Prashant ( _P0W! )
// Lets play sprint poker ...
// dated : November 23rd, 2016.

#include <array>
#include <random>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iterator>
#include <map>
#include <cassert>


class poker
{
public:
    poker() : 
    face("A23456789TJQK"), 
    suit ( "SHCD"), 
    faceCnt{},
    suitCnt{}  
    { 

    
    }
    
    ~poker()
    {
        
    }
    
   int analyze( const std::string& h )
   {
      
      std::vector<std::string> hand;  


      std::memset( faceCnt, 0, sizeof faceCnt );
      std::memset( suitCnt, 0, sizeof suitCnt );
       
      std::istringstream i( h );

      std::copy( std::istream_iterator<std::string>( i ), 
         std::istream_iterator<std::string>(), 
         std::back_inserter<std::vector<std::string> >( hand ) 
         );
      
      if( hand.size() != 5 ) 
         return 10; //"invalid hand."; 

      std::vector<std::string>::iterator it = hand.begin();
      std::sort( it, hand.end() ); 

      if( hand.end() != adjacent_find( it, hand.end() ) ) 
         return 10; //"invalid hand.";

      while( it != hand.end() )
      {
         if( ( *it ).length() != 2 ) 
            return 10; //"invalid hand.";

         int n = face.find( ( *it ).at( 0 ) ) ;

         int l = suit.find( ( *it ).at( 1 ) );

         if( n < 0 || l < 0 ) 
            return 10; // "invalid hand.";
         
        
         faceCnt[n]++; 
         suitCnt[l]++; 

         it++;
      }

      
      return analyzeHand();
   }
private:
   int analyzeHand()
   {
      bool p1 = false, p2 = false, t = false, f = false, fl = false, st = false;

      for( int x = 0; x < 13; x++ )
         switch( faceCnt[x] )
      {
         case 2: 
          if( p1 ) 
          { 
              p2 = true; 
          }
          else 
          {
              p1 = true;
          }
          break;
         case 3: t = true; break;
         case 4: f = true;
      }
      for( int x = 0; x < 4; x++ )if( suitCnt[x] == 5 )
      { 
         fl = true; 
         break; 
      }

      if( !p1 && !p2 && !t && !f )
      {
         int s = 0;
         for( int x = 0; x < 13; x++ )
         { 
            if( faceCnt[x] ) s++; else s = 0;
            if( s == 5 ) break;
         }
         st = ( s == 5 ) || ( s == 4 && faceCnt[0] && !faceCnt[1] );
      }

      if( st && fl ) 
      {
         //A23456789TJQK
         if(  faceCnt[0] && faceCnt[9] && faceCnt[10] && faceCnt[11] && faceCnt[12] )
         {
            return 0; //"royal-flush";
         }

         return  1; //"straight-flush";
      }
      else if( f ) 
         return 2; //"four-of-a-kind"; 
      else if( p1 && t ) 
         return 3 ; //"full-house";
      else if( fl ) 
         return 4; //"flush";
      else if( st ) 
         return 5; //"straight";
      else if( t ) 
         return 6; //"three-of-a-kind";
      else if( p1 && p2 ) 
         return 7; //"two-pair";
      else if( p1 ) 
         return 8; //"one-pair";
      return 9; //"high-card";
   }

   std::string face, suit;
   unsigned char faceCnt[13], suitCnt[4];
};


constexpr const char* const Face = "A23456789TJQK";
constexpr const char* const Suit = "SHCD";
std::string Hands[]=
{
   "royal-flush    ",
   "straight-flush ",
   "four-of-a-kind ",
   "full-house     ",
   "flush          ",
   "straight       ",
   "three-of-a-kind",
   "two-pair       ",
   "one-pair       ",
   "high-hand      ",
   "no hands       ",
};
constexpr  auto generateDeck( void )
{
   std::array< std::pair< char, char> , 52 > deck;
   for( int i = 0; i< 13; ++i )
   {
      for( int j = 0; j< 4; ++j )
      {
         deck[ i*4+j ] = { Face[i],  Suit[j] } ;
      }
   }

   return deck ;

}

std::pair<int, std::string> getBestHand( const std::array< std::pair< char, char>, 52 >& deck,
                                         int startIndex,
                                         int n 
                                       )
{
   startIndex--; 

   assert( startIndex + n <= 52 );

   std::vector<bool> v(n);
   std::fill(v.begin(), v.begin() + 5, true);
   std::string hand = "";
   std::map< int, std::string > best;

   poker p;


   do {
      for (int i = 0; i < n; ++i) 
      {
         if (v[i]) 
         {

            if( hand != "" )
            {
                hand += " " ;
            }
            hand += std::string("") + deck[ startIndex + i ].first    + deck[ startIndex + i ].second ;
         }
      }
      
      
      best.insert( std::make_pair( p.analyze( hand ) , hand  )  ) ;

      hand = "";


   } while (std::prev_permutation(v.begin(), v.end()));

   assert( best.size() != 0 );
    
   return *best.begin() ;
}

   std::map< std::string, int > awards = 
   {
      {"Prashant",    2 },
      {"Craig   ",    22 },
      {"Josh    ",    11 },
      {"Shawn   ",    15 },
      {"Jamie   ",    15 },
      {"Jeff    ",    12 },
      {"Anushree",    19 },
      {"Kishore ",    14 },
      {"Vikram  ",    18 },
      {"Sowmiya ",    13 },          
      {"Akhila  ",    16 },
      {"Dave    ",    13 },
      {"Swarner ",    23 },
   };

int main()
{
    #if 1
    
   std::random_device rd;
   auto startCardNo = 1 ;

   std::mt19937 g(rd());
   auto deck = generateDeck() ;

   std::shuffle(deck.begin(), deck.end(), g);

   for( const auto& player: awards  )
   { 
       if( startCardNo + player.second - 1 > 52 )
       {
           std::cout << "\n Cards Over Reshuffling Deck ... \n\n";
           startCardNo = 1;
           std::shuffle( deck.begin(), deck.end(), g);
           
       }
       
      auto results = getBestHand( deck, startCardNo , player.second  )  ;
       
      std::string hand = "";
      for( int i = 0; i < player.second; ++i )
      {
          hand += std::string( "" ) + deck[ startCardNo + i - 1  ].first   + deck[ startCardNo + i -1 ].second + " ";
      }
        
      std::cout << player.first << " Best : " << Hands[ results.first ]  << " ( " << results.second  << " from { " << hand << " } ) \n" ;
      startCardNo  +=  player.second ;
      
   }
    #endif

    
   return 0;
}

