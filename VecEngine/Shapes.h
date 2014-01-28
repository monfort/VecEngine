/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// Shapes.h: interface for the CShapes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPES_H__E4C3A3F3_C724_11D3_A4A1_00C0CA1261A6__INCLUDED_)
#define AFX_SHAPES_H__E4C3A3F3_C724_11D3_A4A1_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecTypes.h"
#include "VecController.h"

//***********************************************************************************
//***********************************************************************************
extern SHAPE ShapeArrowUp;
extern SHAPE ShapeBell;
extern SHAPE ShapeBird;
extern SHAPE ShapeBlast;
extern SHAPE ShapeBomb;
extern SHAPE ShapeBook;
extern SHAPE ShapeCandle;
extern SHAPE ShapeCat;
extern SHAPE ShapeClub;
extern SHAPE ShapeDiamon;
extern SHAPE ShapeDiamond2;
extern SHAPE ShapeDog;
extern SHAPE ShapeDrop;
extern SHAPE ShapeEye;
extern SHAPE ShapeFinger;
extern SHAPE ShapeFish;
extern SHAPE ShapeFlag;
extern SHAPE ShapeFlower;
extern SHAPE ShapeHamsha;
extern SHAPE ShapeHeart;
extern SHAPE ShapeHouse;
extern SHAPE ShapeLightbulb;
extern SHAPE ShapeLightning;
extern SHAPE ShapeLips;
extern SHAPE ShapeMoon;
extern SHAPE ShapePencil;
extern SHAPE ShapePeper;
extern SHAPE ShapePhone;
extern SHAPE ShapeProfile;
extern SHAPE ShapePulm;
extern SHAPE ShapeSandClock;
extern SHAPE ShapeCards;
extern SHAPE ShapeSqwarl;
extern SHAPE ShapeStar15;
extern SHAPE ShapeStar4;
extern SHAPE ShapeStar5;
extern SHAPE ShapeStar6;
extern SHAPE ShapeSun;
extern SHAPE ShapeSunglasses;
extern SHAPE ShapeTalkBubble;
extern SHAPE ShapeTextBox;
extern SHAPE ShapeThumbUp;
extern SHAPE ShapeTrack;
extern SHAPE ShapeTree;
extern SHAPE ShapeTrofy;
extern SHAPE ShapeVideoCam;
extern SHAPE ShapeWineGlass;

extern SHAPE ShapeTwoheadarrow;
extern SHAPE ShapeThinkingcloud1;
extern SHAPE ShapeTshirt;
extern SHAPE ShapeTurtle;
extern SHAPE ShapeThinkingcloud;
extern SHAPE ShapeTalkbobble3;
extern SHAPE ShapeTalkbobble2;
extern SHAPE ShapeTalkbobble1;
extern SHAPE ShapeSpaceship;
extern SHAPE ShapeSorprise;
extern SHAPE ShapeSnail;
extern SHAPE ShapeShoe;
extern SHAPE ShapeShell;
extern SHAPE ShapeReebon;
extern SHAPE ShapeRabithead;
extern SHAPE Shapequestionmark;
extern SHAPE Shapeplus;
extern SHAPE ShapePicturframe;
extern SHAPE ShapePaper;
/*extern SHAPE ShapeNo9;
extern SHAPE ShapeNo8;
extern SHAPE ShapeNo7;
extern SHAPE ShapeNo6;
extern SHAPE ShapeNo5;
extern SHAPE ShapeNo4;
extern SHAPE ShapeNo3;
extern SHAPE ShapeNo2;
extern SHAPE ShapeNo1;*/
extern SHAPE ShapeMedal;
extern SHAPE ShapeMashroom;
extern SHAPE ShapeKey;
extern SHAPE ShapeIcecream;
extern SHAPE ShapeHeart2;
extern SHAPE ShapeGuitar;
extern SHAPE ShapeGhost;
extern SHAPE ShapeFootprint;
extern SHAPE ShapeFire;
extern SHAPE ShapeDolar;
extern SHAPE ShapeCow;
extern SHAPE ShapeCoopidon;
extern SHAPE ShapeCandy;
extern SHAPE ShapeBearhead;
extern SHAPE ShapeBalloon;
extern SHAPE ShapeApple;
extern SHAPE ShapeAlarmclock;
extern SHAPE ShapeAirplane;
extern SHAPE ShapeAgas;


//***********************************************************************************
//***********************************************************************************
//#define DEBUG_IMPORT_SHAPE

#ifdef DEBUG_IMPORT_SHAPE
void ReadNewShape();
LPSHAPE GetLastShape();
#endif

#endif // !defined(AFX_SHAPES_H__E4C3A3F3_C724_11D3_A4A1_00C0CA1261A6__INCLUDED_)
