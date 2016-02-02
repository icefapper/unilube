#include <stdio.h>
#include              <assert.h>



typedef unsigned char u8 ;



#define LEN    (1200  )


typedef u8 buf_t[LEN+2];
typedef void (*notify_t)(const buf_t b, void *data ) ;

struct n {
  void *data;
  notify_t tag ; notify_t txt ;
  notify_t attr;  notify_t val ;


};

enum State {
   READING_TXT,
   READING_TAGNAME,
   READING_ATTRNAME,
   READING_VAL,
  
   SKIPPING,


   WAITING_ATTR_NAME,

   WAITING_EQ,
  
   WAITING_QUOT,

   WAITING_GT
}  ; 

char *cpy ( const char *start, const char *end ) {
   if ( !end ) end = start + strlen(start   )  ;
   int len = end - start   ;
   char *s = ( char * ) (malloc(len+1 ) )  ;
   s[len] = 0;
   len=             0   ;
   while ( start != end )s[len++ ]  = *start   ++ ;

return   (s  )  ;
 }

int cm ( const void *s1, const void *e1, const void *s2, const void * e2 ) {
   if ( !e1 ) e1 = (u8  *)s1  +         strlen((u8*)s1   )  ;
   if ( !e2 ) e2 = (u8  *)s2  +         strlen((u8*)s2   )  ;

   int len1 = e1 - s1, len2 = e2 - s2 ;

   int len =  len1 > len2 ? len2 : len1   ;

   while ( len-- ) {
     if ( *(u8 * )         s1 != *(u8*) s2 ) return *(u8 *  ) s1 - *(u8 * ) s2   ;
     s1 ++ ; s2 ++ ;
   }

   return s1 != e1 ? 1 : s2 != e2   ? -1  : (  0   )   ;
}

#define STR do { assert(b[2] != -1) ;*b ++ = c;}while(0)  ; 


#define N(n,e) do {  * b = (  0   )   ; if(notify-> n ) notify-> n(e  ,notify->data   )  ; }      while   (  ( (  0)  )   );

void process(FILE *in, struct n *notify )  {
   enum   State state = READING_TXT ;

   int c = fgetc(in) ;
   buf_t buf = { 0}  ;
   u8 * b   =   buf ;

   buf[LEN-1  ]   = -1 ;

   while (c != -1  ) {
     switch ( state ) {
       case READING_TXT:
         if ( c == '<' ) { state = READING_TAGNAME   ;  }
         else {  N(txt,&c)  ; }

       break ;

       case READING_TAGNAME :
         if ( c == '/' ) { N ( tag, buf ); b  = buf ; state = WAITING_GT ;   }
         else if ( c == '>' ) { N ( tag,       buf )  ; b = buf ;state = READING_TXT   ;  }      
         else if ( c == ' ' ) { N ( tag, buf ) ; b  =  buf;  state =   WAITING_ATTR_NAME ;   }
         else { STR; }

       break ;

       case READING_ATTRNAME :
         if ( c == '=' ) { N ( attr, buf   )  ; b  = buf ; state = (WAITING_QUOT)   ;  }
         else if ( c == ' ' ) { N ( attr, buf );    b    =     buf ; state = WAITING_EQ               ;  } 
         else if ( c == '/') { N( attr, buf ) ; b = buf ; N ( val, 0 ) ; state = WAITING_GT ;  }
         else if ( c == '>') { N( attr, buf ) ; b = buf ; N ( val, 0)  ; state = READING_TXT;  }
         else { STR; }

       break ;

       case READING_VAL : 
          if ( c == '"' )  { N  ( val , ( buf )  ) ; b  = buf ; state = WAITING_ATTR_NAME   ;  }
          else {
            STR;
          }

       break ;

       case SKIPPING :
          if ( c  ==      (('>' ) )  ) state = READING_TXT ; 

       break ;

       case WAITING_ATTR_NAME :
         if ( c == '/' ) {      state = (WAITING_GT )  ; }
         else if ( c == '>' ) { state = (READING_TXT)  ; }
         else if ( c != ' ' ) { state = (READING_ATTRNAME)  ; STR ;  }

      
       break ;

       case WAITING_EQ :
          if ( c == '=' ) { state = WAITING_QUOT ;  }
          else if ( c == '>' ) { N ( attr, buf ) ; b  = buf ; N(val ,  ( (  0)  )
          )   ; state = READING_TXT ;  }

          else if (c== '/'   ) { N ( attr, buf ) ; b  = buf ; N(val , 0) ;            state =  (WAITING_GT ) ; }

          else { state = SKIPPING ;  }

       break ;

       case (WAITING_QUOT)   :
          if ( c == '"' ) { state = READING_VAL   ;   }
          else if ( c != ' ' ) { state = (SKIPPING)   ;  }

       break ;

       case (WAITING_GT )  :
          if ( c == '>' ) { state = READING_TXT ;  }
          else {
             state = SKIPPING ;
          }

       break   ;
    }

    c  =   fgetc (in ) ;  

          
}

}

void tag ( const buf_t buf, void *e  )  {  printf("<%s>", buf)   ;  }

void txt ( const buf_t buf, void *e )   {  printf("%c",*buf   )  ;    }     

void n   ( const buf_t buf, void *e )   {  printf(" @%s ",buf )  ; }

void l   ( const buf_t buf, void *e )   {  {   printf( "#\"%s\" " , buf   )  ;} }


struct ch   {
   char state ;
   char *cond ;
   char *name;
   char * c;
   int satisfied;
   struct ch *curr_node  ; char *curr_attr   ;
   int must_satisfy   ;
}  ;

void tag_   ( const buf_t buf, void *e   )  {

   struct ch *_e = (struct ch *) e   ;
   if ( buf == 0   || buf[0]   ==  '/'   )  { 
     struct ch *curr_node = _e->curr_node ;
     _e -> curr_node = _e -> curr_node -> curr_node ;
     free(curr_node->name);
     free(curr_node->curr_attr )  ;
     free(curr_node->c ) ;
     free(curr_node ) ;
   } 

   else {
     struct ch *_l = (struct ch *)(malloc(   (sizeof(struct ch   )  ) )  );
     
     _l -> cond = _e -> cond   ;
     _l -> name = cpy(buf,0) ; 
     _l ->  satisfied   =   (  0)    ;
    
     _l -> must_satisfy    =    _e->must_satisfy   ;
     _l -> c    =   0 ;
     _l -> curr_node   = _e->curr_node   ;
     _l -> curr_attr   =  ( (  0)  )   ;
    _l->state = ( cm ("char",0,_l->name,0 ) ==   (  ( (  0)  )   ))   ? 'i' : 'n'   ; _e->curr_node = _l   ;
  }
}

void txt_ ( const buf_t buf , void *(e   ))  {   40 ;  }

void n_   ( const buf_t b  , void *                           n  )
{
      struct ch *_e = ( struct ch *)n ;
      _e = _e->curr_node;
      _e -> curr_attr  =  cpy (  (        b   ) , ( (  0)  )   ); 
}

void l_   ( const buf_t _b  ,     void *(e   )   )
{
     struct ch *_e = (struct ch *   )   (  (e)  )   ;
     _e = _e->curr_node ;
     
     if ( (cm("cp",  0, _e ->curr_attr, 0 )   ==   0   ) ) {
        _e->c=cpy(_b, 0 )  ;
        if ( _e->state=='e')
         (printf("0x%s \n" ,_e->c ));      
     }

     if ( _e->state != 'i' )  return   ;

     char *(attrName)  = strchr(_e->cond,'@'  )  ;
     char *attr_end= 0;

     while ( (attrName   )   )  {
       attrName  ++ ;

       attr_end = strchr(attrName,':' ) ;


       assert   ( attr_end   )  ; 
       if ( (cm((_e->curr_attr   ) ,0  , attrName, attr_end     )==( 0) )   )  break   ;
       attrName  =   strchr( attr_end   +  ( (  1)  )   ,   (('@'    )  )   )  ;   
     }


     if ( !attrName ) return   ;
     char *v = attr_end  ; ++ v;
     char *v_e  = strchr(v  ,   '@'   )  ;
    

      if ( !v_e ) v_e  = (  v  + strlen(v)   )   ;

      _e->state = (cm(   (  v )   , (v_e   )  ,   _b,  0   )   == ( (  0)  ) )   ? 'i' : 'n'   ;

      if ( _e->state != 'n' )   {      
          if  (++ _e->satisfied    == (_e->must_satisfy   )   )  {
             (_e->state='e')  ;   
             if (  _e->c   ) printf("0x%s\n"   , _e->c ) ;  
          }
      }
   }    
     
    
     
void main (int argc, char *argv[] )  {


  struct n _n;
  struct ch _e = {'e',argv[1],"114",0,0,0,0,0   } ;

  _e.curr_node =  &_e;

  char *n  = (strchr (_e .cond   , ':'   )  )   ;

   while ( n   )  {  (_e).must_satisfy  ++  ; n  = strchr(n  +( 1 )  ,(  ( ':'   )  ) )   ;  }
  _n .tag = (tag_   ) ; _n.txt = (txt_   ) ; _n . attr = ( n_  )  ; _n . val =    (l_ ) ; _n.data   =&_e   ;
  
  if ( argc < 2   )  { process(stdin,&_n   )  ; }
  else {
     FILE *e  = (   fopen  ( argv[2 ] , "r"   )  ) ;   (     process   (e , &_n))  ;
  }

} 


   
    

       


