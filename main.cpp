#include "palindrome.h"
#include "bitmap.h"
#include "unionfind.h"
#include "parenmatch.h"
#include "stringsequence.h"
#include "sort.h"
#include "racetrack.h"
#include "rectanglewalk.h"

void envInit() {
    srand( (unsigned) time( 0 ) );
}

int main()
{
    envInit();

    //BitmapTest::runTest();
    //PalindromeTest::runTest();
    //ParenMatchTest::runTest();
    //StringSequenceTest::runTest();
    //SortTest::runTest();
    //RaceTrackTest::runTest();
    RectangleWalkTest::runTest();
    return 0;
}
