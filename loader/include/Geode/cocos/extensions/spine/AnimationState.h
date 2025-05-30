/*******************************************************************************
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef SPINE_ANIMATIONSTATE_H_
#define SPINE_ANIMATIONSTATE_H_

#include "AnimationStateData.h"

namespace cocos2d { namespace extension {

typedef struct {
	AnimationStateData* const data;
	Animation* const animation;
	float time;
	int/*bool*/loop;
} AnimationState;

/* @param data May be 0 for no mixing. */
AnimationState* AnimationState_create (AnimationStateData* data);
void AnimationState_dispose (AnimationState* self);

void AnimationState_update (AnimationState* self, float delta);

void AnimationState_apply (AnimationState* self, struct Skeleton* skeleton);

/* @param animationName May be 0. */
void AnimationState_setAnimationByName (AnimationState* self, const char* animationName, int/*bool*/loop);
/* @param animation May be 0. */
void AnimationState_setAnimation (AnimationState* self, Animation* animation, int/*bool*/loop);

/** @param animationName May be 0.
 * @param delay May be <= 0 to use duration of previous animation minus any mix duration plus the negative delay. */
void AnimationState_addAnimationByName (AnimationState* self, const char* animationName, int/*bool*/loop, float delay);
/** @param animation May be 0.
 * @param delay May be <= 0 to use duration of previous animation minus any mix duration plus the negative delay. */
void AnimationState_addAnimation (AnimationState* self, Animation* animation, int/*bool*/loop, float delay);

void AnimationState_clearAnimation (AnimationState* self);

int/*bool*/AnimationState_isComplete (AnimationState* self);

}} // namespace cocos2d { namespace extension {

#endif /* SPINE_ANIMATIONSTATE_H_ */
