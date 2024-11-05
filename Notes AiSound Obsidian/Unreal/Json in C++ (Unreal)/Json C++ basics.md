https://www.unrealcode.net/BasicsOfJson.html
# Using the Unreal Engine Json classes

Updated for Unreal Engine 5.0

This document describes the basics of reading and writing Json with Unreal Engine classes. The code here reads and writes elements explicitly rather than reading and writing entire structs at one time. This is suitable for objects which are not using the UCLASS or UOBJECT macros to create reflection data.

## Necessary Includes

Include these header files:

```cpp
#include "Serialization/JsonTypes.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
```

## Using constant field names

To avoid spelling mistakes and make it easier to change Json field names, I suggest making the field names constants like so:

```cpp
namespace JConstants
{
	const char* const ArrayValue("array-value");
	const char* const ArrayValueDisplayName("array-value-display-name");
	const char* const ArrayValues("array-values");
	const char* const AvailableClasses("available-classes");
	const char* const ClassConfigName("class-config-name");
	const char* const ClassName("class-name");
	const char* const ClassDisplayName("class-display-name");
	const char* const ConsoleVariable("console-variable");
	...
}
```

It is a bit more work up front but it reduces errors.

## Creating Json

To create Json make a Json object which will contain everything:

```cpp
TSharedRef< FJsonObject > JsonClassObject = MakeShared<FJsonObject>();
```

Then you can add fields to it using SetXXX methods like so:

```cpp
JsonClassObject->SetStringField( JConstants::ClassName, ClassName);
JsonClassObject->SetBoolField( JConstants::RestartRequired, bRestartRequired);
```

If you need to add a list of child objects, create an array object to hold this list:

```cpp
TArray < TSharedPtr<FJsonValue> > JsonPropertyList;
```

Then create each child object and add it to the list object, then finally add the list object to the containing object like so:

```cpp
TSharedRef< FJsonObject > JsonOuterObject = MakeShared<FJsonObject>();

for( int i = 0; i < 10; ++i )
{
	// make new child object
	TSharedRef< FJsonObject > ChildObject = MakeShared<FJsonObject>();
	// populate the child object
	ChildObject->SetStringField( JConstants::ClassConfigName, ClassConfigName );
	ChildObject->SetStringField( JConstants::ClassDisplayName, ClassDisplayName );
	// add the child object to the collection
	JsonPropertyList.Add(MakeShared<FJsonValueObject>(ChildObject));
}
// add the collection to the outer object which contains it
JsonOuterObject->SetArrayField(JConstants::Properties, JsonPropertyList);
```

### Writing Json to a file

First you need to write the Json object to a string, then serialize that string to file like this:

```cpp
FString OutputString;
TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
FJsonSerializer::Serialize(JsonOuter, Writer);
FFileHelper::SaveStringToFile(OutputString, *AvailableClassesFileName );
Writer->Close();
```

## Reading Json from a file

The entire Json is read from the file into a string, then that string is parsed using the TJsonReader class, then each field and child object can be extracted.

Reading from a file into string is like so:

```cpp
const FString FileName = ResourceDirectory + "\\Index.json";
FString Contents;
if (FFileHelper::LoadFileToString(Contents, *FileName))
{
	...
}
```

Parsing the string into Json uses a TJsonReader and a static call on the FJsonSerializer class:

```cpp
TSharedPtr<FJsonObject> FullJsonObject = MakeShared<FJsonObject>();
TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Contents);

if( FJsonSerializer::Deserialize(Reader, FullJsonObject) )
{
	...
}
```

### Extracting fields from the reader

The most basic approach is to test for the existence of a field and, if it exists, read it from the Json like so:

```cpp
if (JsonObject->HasTypedField<EJson::String>(JConstants::Title))
{
	JsonObject->GetStringField(PropertyName);
}
```

With some templates shown below this can be simplified to one call per field:

```cpp
Description = GetIfExists<FString>(JsonObject, JConstants::Description );
Source = GetIfExists<FString>(JsonObject, JConstants::Source);
SourceURL = GetIfExists<FString>(JsonObject, JConstants::SourceURL );
Version = GetIfExists<FString>(JsonObject, JConstants::Version);
```

The GetIfExists() function uses templates which are specialized for each data type (to save space only the FString and bool specializations are shown below). First the `struct JsonEnum<T>` maps from a c++ data type such as FString to the corresponding EJson enumeration EJson::String.

```cpp
template< typename T >
struct JsonEnum;

template <>
struct JsonEnum<FString>
{
	const static EJson Type = EJson::String;
};

template <>
struct JsonEnum<bool>
{
	const static EJson Type = EJson::Boolean;
};
```

Then the `struct Extractor<T>` maps from a c++ data type such as FString to the corresponding GetXXX() call on the FJsonObject like this:

```cpp
template< typename T >
struct Extractor;

template <>
struct Extractor<FString>
{
	FString operator()(const TSharedPtr<FJsonObject>& JsonObject, 
	                   const FString& PropertyName)
	{
		return JsonObject->GetStringField(PropertyName);
	}
};

template <>
struct Extractor<bool>
{
	bool operator()(const TSharedPtr<FJsonObject>& JsonObject, 
	                const FString& PropertyName)
	{
		return JsonObject->GetBoolField(PropertyName);
	}
};
```

And finally the `GetIfExists()` function uses the two above templates:

```cpp
template< typename T >
T GetIfExists(const TSharedPtr<FJsonObject>& JsonObject, 
			  const FString& PropertyName)
{
	if (JsonObject && JsonObject->HasTypedField< JsonEnum<T>::Type >(PropertyName))
	{
		return Extractor<T>{}(JsonObject, PropertyName);
	}

	return T();
}
```

### Reading arrays of child objects

When the Json object has a child array of other objects, this can be accessed using the GetArrayField method:

```cpp
if (JsonObject->HasTypedField<EJson::Array>(JConstants::Settings))
{
	const TArray<TSharedPtr<FJsonValue>>& Settings = 
			JsonObject->GetArrayField(JConstants::Settings);

	for ( const TSharedPtr<FJsonValue>& Setting : Settings)
	{
		const TSharedPtr<FJsonObject> Object = Setting->AsObject();
		if (Object)
		{
			TSharedRef<FPresetSetting> PresetSetting = MakeShared<FPresetSetting>();
			PresetSetting->ClassConfigName = 
			               GetIfExists<FString>( Object, JConstants::ClassConfig);
			PresetSetting->ClassDisplayName = 
			               GetIfExists<FString>(Object, JConstants::ClassDisplay);
			PresetSetting->PropertyDisplayName = 
			               GetIfExists<FString>(Object, JConstants::DisplayName);
			...
		}
	}
}
```

MIT Licensed | Copyright © 2020-2024 John Farrow : john.farrow@unrealcode.net