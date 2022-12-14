#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_MACOS

#include <cocos2d.h>
using namespace cocos2d;

CCObject* CCObject::copy()
{
    return copyWithZone(0);
}

/** Allocates and initializes a new array with specified capacity */
ccArray* cocos2d::ccArrayNew(unsigned int capacity) 
{
	if (capacity == 0)
		capacity = 1;
	
	ccArray *arr = (ccArray*)malloc( sizeof(ccArray) );
	arr->num = 0;
	arr->arr =  (CCObject**)calloc(capacity, sizeof(CCObject*));
	arr->max = capacity;
	
	return arr;
}

/** Frees array after removing all remaining objects. Silently ignores NULL arr. */
void cocos2d::ccArrayFree(ccArray*& arr)
{
    if( arr == NULL ) 
    {
        return;
    }
	ccArrayRemoveAllObjects(arr);
	
	free(arr->arr);
	free(arr);

    arr = NULL;
}

void cocos2d::ccArrayDoubleCapacity(ccArray *arr)
{
	arr->max *= 2;
	CCObject** newArr = (CCObject**)realloc( arr->arr, arr->max * sizeof(CCObject*) );
	// will fail when there's not enough memory
    CCAssert(newArr != 0, "ccArrayDoubleCapacity failed. Not enough memory");
	arr->arr = newArr;
}

void cocos2d::ccArrayEnsureExtraCapacity(ccArray *arr, unsigned int extra)
{
	while (arr->max < arr->num + extra)
    {
		ccArrayDoubleCapacity(arr);
    }
}

void cocos2d::ccArrayShrink(ccArray *arr)
{
    unsigned int newSize = 0;
	
	//only resize when necessary
	if (arr->max > arr->num && !(arr->num==0 && arr->max==1))
	{
		if (arr->num!=0)
		{
			newSize=arr->num;
			arr->max=arr->num;
		}
		else
		{//minimum capacity of 1, with 0 elements the array would be free'd by realloc
			newSize=1;
			arr->max=1;
		}
		
		arr->arr = (CCObject**)realloc(arr->arr,newSize * sizeof(CCObject*) );
		CCAssert(arr->arr!=NULL,"could not reallocate the memory");
	}
}

/** Returns index of first occurrence of object, CC_INVALID_INDEX if object not found. */
unsigned int cocos2d::ccArrayGetIndexOfObject(ccArray *arr, CCObject* object)
{
    const unsigned int arrNum = arr->num;
    CCObject** ptr = arr->arr;
	for(unsigned int i = 0; i < arrNum; ++i, ++ptr)
    {
		if( *ptr == object ) return i;
    }
    
	return CC_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether object is present in array. */
bool cocos2d::ccArrayContainsObject(ccArray *arr, CCObject* object)
{
	return ccArrayGetIndexOfObject(arr, object) != CC_INVALID_INDEX;
}

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void cocos2d::ccArrayAppendObject(ccArray *arr, CCObject* object)
{
    CCAssert(object != NULL, "Invalid parameter!");
    object->retain();
	arr->arr[arr->num] = object;
	arr->num++;
}

/** Appends an object. Capacity of arr is increased if needed. */
void cocos2d::ccArrayAppendObjectWithResize(ccArray *arr, CCObject* object)
{
	ccArrayEnsureExtraCapacity(arr, 1);
	ccArrayAppendObject(arr, object);
}

/** Appends objects from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void cocos2d::ccArrayAppendArray(ccArray *arr, ccArray *plusArr)
{
	for(unsigned int i = 0; i < plusArr->num; i++)
    {
		ccArrayAppendObject(arr, plusArr->arr[i]);
    }
}

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void cocos2d::ccArrayAppendArrayWithResize(ccArray *arr, ccArray *plusArr)
{
	ccArrayEnsureExtraCapacity(arr, plusArr->num);
	ccArrayAppendArray(arr, plusArr);
}

/** Inserts an object at index */
void cocos2d::ccArrayInsertObjectAtIndex(ccArray *arr, CCObject* object, unsigned int index)
{
	CCAssert(index<=arr->num, "Invalid index. Out of bounds");
	CCAssert(object != NULL, "Invalid parameter!");

	ccArrayEnsureExtraCapacity(arr, 1);
	
	unsigned int remaining = arr->num - index;
	if( remaining > 0)
    {
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(CCObject*) * remaining );
    }

    object->retain();
	arr->arr[index] = object;
	arr->num++;
}

/** Swaps two objects */
void cocos2d::ccArraySwapObjectsAtIndexes(ccArray *arr, unsigned int index1, unsigned int index2)
{
	CCAssert(index1 < arr->num, "(1) Invalid index. Out of bounds");
	CCAssert(index2 < arr->num, "(2) Invalid index. Out of bounds");
	
	CCObject* object1 = arr->arr[index1];
	
	arr->arr[index1] = arr->arr[index2];
	arr->arr[index2] = object1;
}

/** Removes all objects from arr */
void cocos2d::ccArrayRemoveAllObjects(ccArray *arr)
{
	while( arr->num > 0 )
    {
		(arr->arr[--arr->num])->release();
    }
}

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void cocos2d::ccArrayRemoveObjectAtIndex(ccArray *arr, unsigned int index, bool bReleaseObj/* = true*/)
{
    CCAssert(arr && arr->num > 0 && index < arr->num, "Invalid index. Out of bounds");
    if (bReleaseObj)
    {
        CC_SAFE_RELEASE(arr->arr[index]);
    }
    
	arr->num--;
	
	unsigned int remaining = arr->num - index;
	if(remaining>0)
    {
		memmove((void *)&arr->arr[index], (void *)&arr->arr[index+1], remaining * sizeof(CCObject*));
    }
}

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void cocos2d::ccArrayFastRemoveObjectAtIndex(ccArray *arr, unsigned int index)
{
	CC_SAFE_RELEASE(arr->arr[index]);
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

void cocos2d::ccArrayFastRemoveObject(ccArray *arr, CCObject* object)
{
	unsigned int index = ccArrayGetIndexOfObject(arr, object);
	if (index != CC_INVALID_INDEX)
    {
		ccArrayFastRemoveObjectAtIndex(arr, index);
    }
}

/** Searches for the first occurrence of object and removes it. If object is not
 found the function has no effect. */
void cocos2d::ccArrayRemoveObject(ccArray *arr, CCObject* object, bool bReleaseObj/* = true*/)
{
	unsigned int index = ccArrayGetIndexOfObject(arr, object);
	if (index != CC_INVALID_INDEX)
    {
		ccArrayRemoveObjectAtIndex(arr, index, bReleaseObj);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void cocos2d::ccArrayRemoveArray(ccArray *arr, ccArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		ccArrayRemoveObject(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void cocos2d::ccArrayFullRemoveArray(ccArray *arr, ccArray *minusArr)
{
	unsigned int back = 0;
	unsigned int i = 0;
	
	for( i = 0; i < arr->num; i++) 
    {
		if( ccArrayContainsObject(minusArr, arr->arr[i]) ) 
        {
			CC_SAFE_RELEASE(arr->arr[i]);
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

/** Allocates and initializes a new C array with specified capacity */
ccCArray* cocos2d::ccCArrayNew(unsigned int capacity)
{
	if (capacity == 0)
    {
		capacity = 1;
    }

	ccCArray *arr = (ccCArray*)malloc( sizeof(ccCArray) );
	arr->num = 0;
	arr->arr = (void**)malloc( capacity * sizeof(void*) );
	arr->max = capacity;
	
	return arr;
}

/** Frees C array after removing all remaining values. Silently ignores NULL arr. */
void cocos2d::ccCArrayFree(ccCArray *arr)
{
    if( arr == NULL ) 
    {
        return;
    }
	ccCArrayRemoveAllValues(arr);
	
	free(arr->arr);
	free(arr);
}

/** Doubles C array capacity */
void cocos2d::ccCArrayDoubleCapacity(ccCArray *arr)
{
    ccArrayDoubleCapacity((ccArray*)arr);
}

/** Increases array capacity such that max >= num + extra. */
void cocos2d::ccCArrayEnsureExtraCapacity(ccCArray *arr, unsigned int extra)
{
    ccArrayEnsureExtraCapacity((ccArray*)arr,extra);
}

/** Returns index of first occurrence of value, CC_INVALID_INDEX if value not found. */
unsigned int cocos2d::ccCArrayGetIndexOfValue(ccCArray *arr, void* value)
{
	unsigned int i;
	
	for( i = 0; i < arr->num; i++)
    {
		if( arr->arr[i] == value ) return i;
    }
	return CC_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool cocos2d::ccCArrayContainsValue(ccCArray *arr, void* value)
{
	return ccCArrayGetIndexOfValue(arr, value) != CC_INVALID_INDEX;
}

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void cocos2d::ccCArrayInsertValueAtIndex( ccCArray *arr, void* value, unsigned int index)
{
	CCAssert( index < arr->max, "ccCArrayInsertValueAtIndex: invalid index");
	
	unsigned int remaining = arr->num - index;
    // make sure it has enough capacity
    if (arr->num + 1 == arr->max)
    {
        ccCArrayDoubleCapacity(arr);
    }
	// last Value doesn't need to be moved
	if( remaining > 0) {
		// tex coordinates
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(void*) * remaining );
	}
	
	arr->num++;
	arr->arr[index] = value;
}

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void cocos2d::ccCArrayAppendValue(ccCArray *arr, void* value)
{
	arr->arr[arr->num] = value;
	arr->num++;
    // double the capacity for the next append action
    // if the num >= max
    if (arr->num >= arr->max)
    {
        ccCArrayDoubleCapacity(arr);
    }
}

/** Appends an value. Capacity of arr is increased if needed. */
void cocos2d::ccCArrayAppendValueWithResize(ccCArray *arr, void* value)
{
	ccCArrayEnsureExtraCapacity(arr, 1);
	ccCArrayAppendValue(arr, value);
}


/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void cocos2d::ccCArrayAppendArray(ccCArray *arr, ccCArray *plusArr)
{
	unsigned int i;
	
	for( i = 0; i < plusArr->num; i++)
    {
		ccCArrayAppendValue(arr, plusArr->arr[i]);
    }
}

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void cocos2d::ccCArrayAppendArrayWithResize(ccCArray *arr, ccCArray *plusArr)
{
	ccCArrayEnsureExtraCapacity(arr, plusArr->num);
	ccCArrayAppendArray(arr, plusArr);
}

/** Removes all values from arr */
void cocos2d::ccCArrayRemoveAllValues(ccCArray *arr)
{
	arr->num = 0;
}

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void cocos2d::ccCArrayRemoveValueAtIndex(ccCArray *arr, unsigned int index)
{
	unsigned int last;
	
	for( last = --arr->num; index < last; index++)
    {
		arr->arr[index] = arr->arr[index + 1];
    }
}

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void cocos2d::ccCArrayFastRemoveValueAtIndex(ccCArray *arr, unsigned int index)
{
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

/** Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void cocos2d::ccCArrayRemoveValue(ccCArray *arr, void* value)
{
	unsigned int index = ccCArrayGetIndexOfValue(arr, value);
	if (index != CC_INVALID_INDEX)
    {
		ccCArrayRemoveValueAtIndex(arr, index);
    }
}

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void cocos2d::ccCArrayRemoveArray(ccCArray *arr, ccCArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		ccCArrayRemoveValue(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void cocos2d::ccCArrayFullRemoveArray(ccCArray *arr, ccCArray *minusArr)
{
	unsigned int back = 0;
	
	for(unsigned int i = 0; i < arr->num; i++) 
    {
		if( ccCArrayContainsValue(minusArr, arr->arr[i]) ) 
        {
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

CCArray::CCArray()
: data(NULL)
{
    init();
}

CCArray::CCArray(unsigned int capacity)
: data(NULL)
{
    initWithCapacity(capacity);
}

CCArray* CCArray::create()
{
    CCArray* pArray = new CCArray();

    if (pArray && pArray->init())
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }
    
    return pArray;
}

CCArray* CCArray::createWithObject(CCObject* pObject)
{
    CCArray* pArray = new CCArray();

    if (pArray && pArray->initWithObject(pObject))
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }

    return pArray;
}

CCArray* CCArray::create(CCObject* pObject, ...)
{
    va_list args;
    va_start(args,pObject);

    CCArray* pArray = create();
    if (pArray && pObject)
    {
        pArray->addObject(pObject);
        CCObject *i = va_arg(args, CCObject*);
        while(i) 
        {
            pArray->addObject(i);
            i = va_arg(args, CCObject*);
        }
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }

    va_end(args);

    return pArray;
}

CCArray* CCArray::createWithArray(CCArray* otherArray)
{
    CCArray* pRet = (CCArray*)otherArray->copy();
    pRet->autorelease();
    return pRet;
}

CCArray* CCArray::createWithCapacity(unsigned int capacity)
{
    CCArray* pArray = new CCArray();
    
    if (pArray && pArray->initWithCapacity(capacity))
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }
    
    return pArray;
}

CCArray* CCArray::createWithContentsOfFile(const char* pFileName)
{
    CCArray* pRet = CCArray::createWithContentsOfFileThreadSafe(pFileName);
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

CCArray* CCArray::createWithContentsOfFileThreadSafe(const char* pFileName)
{
    return CCFileUtils::sharedFileUtils()->createCCArrayWithContentsOfFile(pFileName);
}

bool CCArray::init()
{
    return initWithCapacity(1);
}

bool CCArray::initWithObject(CCObject* pObject)
{
    ccArrayFree(data);
    bool bRet = initWithCapacity(1);
    if (bRet)
    {
        addObject(pObject);
    }
    return bRet;
}

/** Initializes an array with some objects */
bool CCArray::initWithObjects(CCObject* pObject, ...)
{
    ccArrayFree(data);
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(pObject == NULL);

        va_list args;
        va_start(args, pObject);

        if (pObject)
        {
            this->addObject(pObject);
            CCObject* i = va_arg(args, CCObject*);
            while(i) 
            {
                this->addObject(i);
                i = va_arg(args, CCObject*);
            }
            bRet = true;
        }
        va_end(args);

    } while (false);

    return bRet;
}

bool CCArray::initWithCapacity(unsigned int capacity)
{
    ccArrayFree(data);
    data = ccArrayNew(capacity);
    return true;
}

bool CCArray::initWithArray(CCArray* otherArray)
{
    ccArrayFree(data);
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! initWithCapacity(otherArray->data->num));

        addObjectsFromArray(otherArray);
        bRet = true;
    } while (0);
    
    return bRet;
}

unsigned int CCArray::count() const
{
    return data->num;
}

unsigned int CCArray::capacity() const
{
    return data->max;
}

unsigned int CCArray::indexOfObject(CCObject* object) const
{
    return ccArrayGetIndexOfObject(data, object);
}

CCObject* CCArray::objectAtIndex(unsigned int index)
{
    CCAssert(index < data->num, "index out of range in objectAtIndex()");

    return data->arr[index];
}

CCObject* CCArray::lastObject()
{
    if( data->num > 0 )
        return data->arr[data->num-1];

    return NULL;
}

CCObject* CCArray::randomObject()
{
    if (data->num==0)
    {
        return NULL;
    }

    float r = ((float)rand()/(float)RAND_MAX);
    
    if (r == 1) // to prevent from accessing data-arr[data->num], out of range.
    {
        r = 0;
    }
    
    return data->arr[(int)(data->num * r)];
}

bool CCArray::containsObject(CCObject* object) const
{
    return ccArrayContainsObject(data, object);
}

bool CCArray::isEqualToArray(CCArray* otherArray)
{
    for (unsigned int i = 0; i< this->count(); i++)
    {
        if (!this->objectAtIndex(i)->isEqual(otherArray->objectAtIndex(i)))
        {
            return false;
        }
    }
    return true;
}

void CCArray::addObject(CCObject* object)
{
    ccArrayAppendObjectWithResize(data, object);
}

void CCArray::addObjectsFromArray(CCArray* otherArray)
{
    ccArrayAppendArrayWithResize(data, otherArray->data);
}

void CCArray::insertObject(CCObject* object, unsigned int index)
{
    ccArrayInsertObjectAtIndex(data, object, index);
}

void CCArray::removeLastObject(bool bReleaseObj)
{
    CCAssert(data->num, "no objects added");
    ccArrayRemoveObjectAtIndex(data, data->num-1, bReleaseObj);
}

void CCArray::removeObject(CCObject* object, bool bReleaseObj/* = true*/)
{
    ccArrayRemoveObject(data, object, bReleaseObj);
}

void CCArray::removeObjectAtIndex(unsigned int index, bool bReleaseObj)
{
    ccArrayRemoveObjectAtIndex(data, index, bReleaseObj);
}

void CCArray::removeObjectsInArray(CCArray* otherArray)
{
    ccArrayRemoveArray(data, otherArray->data);
}

void CCArray::removeAllObjects()
{
    ccArrayRemoveAllObjects(data);
}

void CCArray::fastRemoveObjectAtIndex(unsigned int index)
{
    ccArrayFastRemoveObjectAtIndex(data, index);
}

void CCArray::fastRemoveObject(CCObject* object)
{
    ccArrayFastRemoveObject(data, object);
}

void CCArray::exchangeObject(CCObject* object1, CCObject* object2)
{
    unsigned int index1 = ccArrayGetIndexOfObject(data, object1);
    if(index1 == UINT_MAX)
    {
        return;
    }

    unsigned int index2 = ccArrayGetIndexOfObject(data, object2);
    if(index2 == UINT_MAX)
    {
        return;
    }

    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray::exchangeObjectAtIndex(unsigned int index1, unsigned int index2)
{
    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray::replaceObjectAtIndex(unsigned int index, CCObject* pObject, bool bReleaseObject/* = true*/)
{
    ccArrayInsertObjectAtIndex(data, pObject, index);
    ccArrayRemoveObjectAtIndex(data, index+1);
}

void CCArray::reverseObjects()
{
    if (data->num > 1)
    {
        // floorf(), since in the case of an even number, the number of swaps stays the same
        int count = (int) floorf(data->num/2.f); 
        unsigned int maxIndex = data->num - 1;

        for (int i = 0; i < count ; i++)
        {
            ccArraySwapObjectsAtIndexes(data, i, maxIndex);
            maxIndex--;
        }
    }
}

void CCArray::reduceMemoryFootprint()
{
    ccArrayShrink(data);
}

CCArray::~CCArray()
{
    ccArrayFree(data);
}

CCObject* CCArray::copyWithZone(CCZone* pZone)
{
    CCAssert(pZone == NULL, "CCArray should not be inherited.");
    CCArray* pArray = new CCArray();
    pArray->initWithCapacity(this->data->num > 0 ? this->data->num : 1);

    CCObject* pObj = NULL;
    CCObject* pTmpObj = NULL;
    CCARRAY_FOREACH(this, pObj)
    {
        pTmpObj = pObj->copy();
        pArray->addObject(pTmpObj);
        pTmpObj->release();
    }
    return pArray;
}

void CCArray::acceptVisitor(CCDataVisitor &visitor)
{
    visitor.visit(this);
}

#endif
