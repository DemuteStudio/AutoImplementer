To manipulate Json in a C++ script, we need to |include the required dependencies ([[Json C++ basics|tutorial]]) : 
```cpp
#include "Serialization/JsonTypes.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
```

**or** we can use the hereditary functions of the FJsonObject class for Unreal Engine C++ : 
![](https://www.youtube.com/watch?v=4wJ45mrzrkM)

If we want to work in blueprints, we'll need an exterior plugin (VaRest) : ![](https://www.youtube.com/watch?v=O8Rskgku8j4)
