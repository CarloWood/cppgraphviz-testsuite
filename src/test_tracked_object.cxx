#include "sys.h"
#include "debug.h"
#include "threadsafe/TrackedObject.h"
#include "threadsafe/ObjectTracker.h"
#include <mutex>

#define UNLOCKED_TYPE_IS_TYPEDEF 0
#define TRACKER_IS_TYPEDEF 1

// Forward declare the object that should be tracked and protected against concurrent access.
class locked_A;

#if UNLOCKED_TYPE_IS_TYPEDEF
// Define the Unlocked version (using the desired locking policy).
using A = threadsafe::UnlockedTrackedObject<locked_A, threadsafe::policy::Primitive<std::mutex>>;
#else
class A;
#endif

#if TRACKER_IS_TYPEDEF
// Define a corresponding tracker class.
using ATracker = threadsafe::ObjectTracker<A, locked_A, threadsafe::policy::Primitive<std::mutex>>;
#else
class ATracker : public threadsafe::ObjectTracker<A, locked_A, threadsafe::policy::Primitive<std::mutex>>
{
 public:
  using threadsafe::ObjectTracker<A, locked_A, threadsafe::policy::Primitive<std::mutex>>::ObjectTracker;
};
// Note: if you run into the compile error 'incomplete locked_Node' during the
// instantiation of the destructor of this NodeTracker class, then move its
// constructors and destructor out of the header (to a .cxx file).
#endif

// The to-be-tracked object must be derived from threadsafe::TrackedObject,
// passing both, the Unlocked type as well as the tracker class.
class locked_A : public threadsafe::TrackedObject<A, ATracker>
{
 public:
  using threadsafe::TrackedObject<A, ATracker>::TrackedObject;
};

#if !UNLOCKED_TYPE_IS_TYPEDEF
class A : public threadsafe::UnlockedTrackedObject<locked_A, threadsafe::policy::Primitive<std::mutex>>
{
 public:
  using threadsafe::UnlockedTrackedObject<locked_A, threadsafe::policy::Primitive<std::mutex>>::UnlockedTrackedObject;
};
#endif

#include "threadsafe/ObjectTracker.inl.h"

int main()
{
  //============================
  // Test UnlockedTrackedObject
  //============================
  A a;

  //============================
  // Test TrackedObject
  //============================
  locked_A locked_a;
}
