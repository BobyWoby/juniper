// "words.txt" was generated from the 5 letter
// words listed @ http://www.math.toronto.edu/~jjchew/scrabble/lists/common-5.html

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



// the zlib.h header file contains
// function prototypes and a lot
// of information about zlib!
#include "zlib.h"

// link the static lib
#pragma comment (lib, "zlibdll/lib/zdll.lib" )

using BYTE = unsigned char;

int main()
{
    srand(time(0));      // seed random number generator

    bool showOutput;     // make false if you don't want text output
    printf( "Show textual output?  [y/n]\n");
    

    printf("*********************\n");
    printf("* zlib test file\n");
    printf("* Using version %s of zlib\n", zlibVersion() ) ;

    #pragma region _make some data to compress_
    const int NUM_WORDS = 8938;     // pre-counted constant

    char * randomWords[ NUM_WORDS ] = {
    };


    const int NUM_WORDS_TO_USE = 80;

    // Fill dataOriginal with a random jumbling of
    // 80 * (5 letter words + 1 space)
    // (each word followed by a space).
    int sizeDataOriginal = 6 * NUM_WORDS_TO_USE + 1 ;
    printf("* Before compression:  your data is %d bytes\n", sizeDataOriginal );
    BYTE * dataOriginal = (BYTE*)malloc( sizeDataOriginal );
    
    for( int i = 0; i < 6 * NUM_WORDS_TO_USE; i += 6 )
    {
        int randomIndex = rand() % NUM_WORDS ;
        static char buf[6];
        strncpy( buf, randomWords[ randomIndex ], 5 );  // don't copy the NULL character.
        buf[5] = ' ';
        strncpy( (char*)(dataOriginal+i), buf, 6 );
    }

    dataOriginal[ sizeDataOriginal - 1 ] = NULL; // null terminator.
    // test it.
    printf("* \n* Here's the data we generated for you:\n\n--\n");
    if( showOutput )
    {
        for( int i = 0 ; i < sizeDataOriginal ; i++ )
        {
            putchar( dataOriginal[i] ); // coulda puts'd this
            // but wanted to be consistent.
        }
        printf("\n--\n\n");
    }
    else
        printf(" (suppressed)\n--\n\n");
    #pragma endregion

    #pragma region compress the data
    //////////////
    // compress it.
    // To compress some data, we'll use the compress()
    // function.

    // To use the compress function, we must
    // create a destination buffer to
    // hold the compressed data.

    // So how big should the compressed
    // data buffer be?
    
    // This may seem a bit weird at first,
    // but the array that is to hold the compressed
    // data must start out being AT LEAST 0.1% larger than
    // the original size of the data, + 12 extra bytes.
    
    // So, we'll just play it safe and alloated 1.1x
    // as much memory + 12 bytes (110% original + 12 bytes)

    unsigned long sizeDataCompressed  = (sizeDataOriginal * 1.1) + 12;

    BYTE * dataCompressed = (BYTE*)malloc( sizeDataCompressed );

    // Now hold on, you ask.  WHY is the array
    // that's supposed to hold the COMPRESSED 
    // data ALREADY BIGGER than the original
    // data array?  This isn't compression!
    // This is meaningless expansion!
    
    // Well, you'll see that this extra space
    // in the compressed array is only TEMPORARY.
    // Just suffice it to say that zlib
    // "needs room to breathe".

    // When zlib performs compression, it will
    // need a bit of extra room to do its work.

    // When the compress() routine returns,
    // the compressedData array will have
    // been AUTOMATICALLY RESIZED by ZLIB
    // to being a smaller, compressed size.

    // We will also know the EXACT size of
    // that compressed data by looking at
    // the 'sizeDataCompressed' variable
    // AFTER the compress() routine runs.
    // That variable 'sizeDataCompressed'
    // will updated by the compress() 
    // function when we call it!
    
    // Don't worry, the "compressed" data
    // will be smaller than the original
    // data was in the end!
    int z_result = compress(
        
        dataCompressed,         // destination buffer,
                                // must be at least
                                // (1.01X + 12) bytes as large
                                // as source.. we made it 1.1X + 12bytes

        &sizeDataCompressed,    // pointer to var containing
                                // the current size of the
                                // destination buffer.
                                // WHEN this function completes,
                                // this var will be updated to
                                // contain the NEW size of the
                                // compressed data in bytes.

        dataOriginal,           // source data for compression
        
        sizeDataOriginal ) ;    // size of source data in bytes

    switch( z_result )
    {
    case Z_OK:
        printf("***** SUCCESS! *****\n");
        break;

    case Z_MEM_ERROR:
        printf("out of memory\n");
        exit(1);    // quit.
        break;

    case Z_BUF_ERROR:
        printf("output buffer wasn't large enough!\n");
        exit(1);    // quit.
        break;
    }
     
    printf("*******************************\n");
    printf("* DATA COMPRESSION COMPLETE!! :\n");
    printf("*\n");
    printf("* Compressed size is %d bytes\n", sizeDataCompressed );
    printf("* This is what it looks like:\n\n--\n");

    // Now we want to print the compressed data out.
    // Can't just printf() it because 
    // the nulls will be all over the place, and there
    // isn't necessarily a null at the end.
    if( showOutput )
    {
        for( int i = 0; i < sizeDataCompressed; i++ )
        {
            putchar( dataCompressed[i] );
        }
        printf("\n--\n\n");
    }
    else
        printf(" (suppressed)\n--\n\n");
    #pragma endregion
    
    #pragma region save compressed data to disk
    //////////////////
    // Save that compressed data to disk.
    // Maybe its save game information or
    // something.
    printf("********************************\n");
    printf("* Saving compressed data to disk\n\n");

    ///////////////////////////////////////////////
    //////////////!!!!!!!!!!!!!!!!!!!!/////////////
    //////////////!!!IMPORTANT NOTE!!!/////////////
    // You see how I'm opening the file in "wb" mode,
    // NOT just "w" mode?????
    FILE * out = fopen( "savedData.dat", "wb" );
    // Well, "wb" means BINARY MODE for writing.
    // THIS IS EXTREMELY IMPORTANT.  IF YOU DON'T
    // USE "wb" TO WRITE YOUR FILES, AND YOU DON'T
    // USE "rb" TO READ YOUR FILES, YOU FILE
    // WILL BE VERY SLIGHTLY CORRUPTED,
    // WINDOWS PROMISES YOU THAT!!

    // One of the reasons for the corruption is
    // under Windows, if you write out a
    // NEWLINE (ASCII code 10, escape sequence "\n",
    // aka the "LINEFEED" (LF)) to the output stream, 
    // then Windows puts out a 13 FIRST, then a 10.

    // 13 is the ascii code for CARRIAGE RETURN (CR),
    // and 10 is ascii code for the Linefeed (LF).

    // Now that seems awfully stupid.  Why would Windows
    // output hex 0D 0A when I clearly asked it to
    // output just 0A?

    // The answer is that Windows uses TWO CHARACTERS
    // to designate the new line:  (CRLF), NOT just LF.

    // Unix uses just LF.  Macs use just CR.  (Windows is
    // the only weirdo that uses 2 characters for a newline.
    // I suppose you could say that this is more true to the
    // original typewriter (or the dot matrix printer),
    // where you have to push the "carriage" (thing that types)
    // back to the left side (carriage return), then
    // you have to feed the paper up a line (linefeed)).

    // SO, this is significant because everytime you write
    // the integer value 10 to the output stream, IF you're
    // NOT writing in binary mode, Windows thinks you're
    // trying to write text.  So NATURALLY, if you write
    // the integer value 10 out in one of the bytes, 
    // Windows recognizes this is a linefeed and
    // write out 13 10 instead.

    // This introduces extra data into your output
    // and is enough to significantly corrupt the result.

    if( out == NULL )
    {
        printf("Couldn't open output file for writing\n");
        exit(1);    //quit
    }
    fwrite( dataCompressed, sizeDataCompressed, 1, out );
    fclose( out );
    out = NULL;
    #pragma endregion

    #pragma region read in data from disk
    ///////////////
    // Next, we'll READ the compressed data
    // from the file, then DECOMPRESS it, to
    // prove that it'll be the same as
    // the original data.
    printf("********************************\n");
    printf("* Reading in data from save file\n");
    
    //////////////!!!!!!!!!!IMPORTANT:  note "rb"
    // NOT just "r".
    FILE * readFile = fopen("savedData.dat", "rb");
    if( readFile == NULL )
    {
        printf("Couldn't open input file for reading\n");
        exit(1);    //quit
    }

    // get size of file
    fseek( readFile, 0, SEEK_END );
    ULONG fileLength = ftell( readFile );
    rewind( readFile );

    // allocate enough mems to hold entire file
    // alternatively, we could "memory map" the
    // file contents using the CreateFileMapping and
    // MapViewOfFile funcs.
    BYTE * dataReadInCompressed = (BYTE*)malloc( fileLength );

    // read in entire file
    fread( dataReadInCompressed, fileLength, 1, readFile );

    // close file
    fclose( readFile );
    readFile = NULL;

    printf("*\n* This is what I read from the saved file:\n"); 
    printf("\n--\n");
    if( showOutput )
    {
        for( int i = 0 ; i < fileLength ; i++ )
        {
            putchar( dataReadInCompressed[i] );
        }
        printf("\n--\n\n");
    }
    else
        printf(" (suppressed)\n--\n\n");
    #pragma endregion

    #pragma region decompress the read-in data
    ///////////////
    // Next, we'll decompress that
    // data we just read in from disk.

    // How large should we make the array
    // into which the UNZIPPED/UNCOMPRESSED
    // data will go?

    // WELL, there's the catch with ZLIB.
    // You never know how big compressed data
    // will blow out to be.  It can blow up
    // to being anywhere from 2 times as big,
    // or it can be (exactly the same size),
    // or it can be up to 10 times as big
    // or even bigger!
    
    // So, you can tell its a really bad idea
    // to try to GUESS the proper size that the
    // uncompressed data will end up being.

    // You're SUPPOSED TO HAVE SAVED THE INFORMATION
    // about the original size of the data at
    // the time you compress it.
    
    // There's a note on how to do that easily
    // at the bottom of this file, in the end notes.

    // FOR NOW, we're just going to 
    // use the dataSizeOriginal variable.
    printf("*******************************\n");
    printf("* Decompressing your data . . .\n");
    ULONG sizeDataUncompressed = sizeDataOriginal;
    BYTE * dataUncompressed = (BYTE*)malloc( sizeDataUncompressed );
    
    //////////////
    // now uncompress
    z_result = uncompress(
        
        dataUncompressed,       // destination for the uncompressed
                                // data.  This should be the size of
                                // the original data, which you should
                                // already know.

        &sizeDataUncompressed,  // length of destination (uncompressed)
                                // buffer

        dataReadInCompressed,   // source buffer - the compressed data

        sizeDataCompressed );   // length of compressed data in bytes

    switch( z_result )
    {
    case Z_OK:
        printf("***** SUCCESS! *****\n");
        break;

    case Z_MEM_ERROR:
        printf("out of memory\n");
        exit(1);    // quit.
        break;

    case Z_BUF_ERROR:
        printf("output buffer wasn't large enough!\n");
        exit(1);    // quit.
        break;
    }


    printf("************************\n");
    printf("* Uncompressed size is %d bytes\n", sizeDataUncompressed );
    printf("* Your UNCOMPRESSED data looks like this:\n");

    printf("\n--\n");
    if( showOutput )
    {
        for( int i = 0 ; i < sizeDataUncompressed ; i++ )
        {
            putchar( dataUncompressed[i] );
        }
        printf("\n--\n\n");
    }
    else
        printf(" (suppressed)\n--\n\n");
    #pragma endregion

    #pragma region compare decompressed data with original data
    if( memcmp( dataOriginal, dataUncompressed, sizeDataOriginal ) == 0 )
    {
        printf("* SEE?  It was EXACTLY the same.\n");
    }
    else
    {
        printf( "\n\n=====================================\n"
                "Oh. . . dear.  There is a problem.  The uncompressed data "
                "isn't exactly the same as the original data.  Your data "
                "may be corrupted.  WHOOPS!!\n"
                "Please make sure if that you are reading and writing "
                "any file i/o in BINARY MODE." );
    }
    #pragma endregion

    free( dataOriginal );
    free( dataCompressed );
    free( dataReadInCompressed );
    free( dataUncompressed );
    
}

/////////
// END NOTES:

// So in this tutorial, when I saved the
// data out to a file, I just used fwrite()
// and dumped the array of bytes with no
// information ABOUT what the file is
// whatsoever.

// in real life, that's probably not a good idea.
// You want to always output at the beginning
// of your compressed data AT LEAST the size
// the data will be when it becomes uncompressed.
// This is the only way you can know how big
// to make the "receiving array" for the uncompressed data.

// ZLIB has no "tell_me_the_size_of_this_thing_
// when_it_gets_uncompressed() function.

// You MUST keep that data in the file itself.

// So, you might create a structure like thus
// and like so:

/*

struct saveFile
{
    ULONG compressedSize;   // size of the data array, compressed
    ULONG uncompressedSize; // size when data gets uncompressed
    char what[16];          // what is this file?  may want to
                            // indicate what kind of data is saved here.

    // add whatever other info you need
    // here.

    BYTE * data;            // the actual compressed data

};
