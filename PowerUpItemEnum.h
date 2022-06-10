// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDash.h"

/**
 * 
 */

UENUM()
enum class EPowerUp :uint8
{
	SPEED = 1 UMETA(DisplayName = "Speed"),
	SMASH = 2 UMETA(DisplayName = "Smash"),
	MAGNET = 3 UMETA(DidplayName = "Magnet")
};