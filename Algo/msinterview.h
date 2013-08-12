/*
Microsoft Interview Questions: Software Development Engineer (SDE)

    You’re given an array containing both positive and negative integers and required to find the sub-array with the largest sum (O(N) a la KBL). Write a routine in C for the above.
    Given an array of size N in which every number is between 1 and N, determine if there are any duplicates in it. You are allowed to destroy the array if you like. [ I ended up giving about 4 or 5 different solutions for this, each supposedly better than the others ].
    Write a routine to draw a circle (x ** 2 + y ** 2 = r ** 2) without making use of any floating point computations at all.
    Given only putchar (no sprintf, itoa, etc.) write a routine putlong that prints out an unsigned long in decimal.
    Give a one-line C expression to test whether a number is a power of 2. No loops allowed.
    Given an array of characters which form a sentence of words, give an efficient algorithm to reverse the order of the words (not characters) in it.
    Give a good method to count the number of ones in a “n” (e.g. 32) bit number.
    What are the different ways to implement a condition where the value of x can be either a 0 or a 1?
    Reverse a linked list.
    Insert in a sorted list.
    */
#pragma once
#include<iostream>

using namespace std;

namespace MS{
    void drawCircle(int r){
        int N = 2*r+1;
        int x, y;
        // Draw a square of size N*N.
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                // Start from the left most corner point
                x = i-r;
                y = j-r;
                // If this point is inside the circle, print it
                if (x*x + y*y <= r*r+1 )
                    cout<<".";
                else // If outside the circle, print space
                    cout<<" ";
                cout<<" ";
            }
            cout<<endl;
        }
    }
}


