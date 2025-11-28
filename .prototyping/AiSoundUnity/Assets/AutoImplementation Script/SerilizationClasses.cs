using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using UnityEditor;

public class AiSoundConverter : JsonConverter
{
    public override bool CanConvert(Type objectType)
    {
        return typeof(AiSound).IsAssignableFrom(objectType);       
    }

    public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
    {
        AiSound soundContainer = null;
        if (reader.TokenType == JsonToken.StartObject)
        {
            JObject infoToRead = JObject.Load(reader);

            string type = infoToRead["type"].Value<string>();

            switch (type)
            {
                case "SimpleSound":
                    soundContainer = ScriptableObject.CreateInstance<SimpleSound>();
                    break;
                case "RandomSound":
                    soundContainer = ScriptableObject.CreateInstance<RandomSound>();
                    break;
                case "SwitchSound":
                    soundContainer = ScriptableObject.CreateInstance<SwitchSound>();
                    break;
                case "SoundEvent":
                    soundContainer = ScriptableObject.CreateInstance<SoundEvent>();
                    break;
                case "SoundBank":
                    soundContainer = ScriptableObject.CreateInstance<SoundBank>();
                    break;
                default:
                    throw new JsonException($"Unknown AiSound type: {type}");
            }

            serializer.Populate(infoToRead.CreateReader(), soundContainer);
        }
        return soundContainer;
    }

    public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
    {
        AiSound soundContainer = (AiSound)value;

        // Create a new JObject to hold the type information
        JObject infoToWrite = new JObject
        {
            { "type", soundContainer.GetType().Name }
        };

        JObject soundObjectInfo = new JObject();

        //Adds EventName to the top of the json structure if it is an Event
        if (typeof(AiSoundEvent).IsAssignableFrom(soundContainer.GetType()))
        {
            AiSoundEvent soundEvent = soundContainer as AiSoundEvent;
            soundObjectInfo.Add("eventName", soundEvent.eventName);
        }
        
        if(typeof(SimpleSound).IsAssignableFrom(soundContainer.GetType()))
        {
            SimpleSound simpleSound = soundContainer as SimpleSound;
            simpleSound.soundPath = "Audio Files/" + simpleSound.soundFile.name;
        }

        // Serialize the object's public fields and properties (excluding Unity internal fields)
        foreach (var field in soundContainer.GetType().GetFields())
        {
            // Skip Unity's internal fields like 'name' and 'hideFlags'
            if (field.Name != "name" && field.Name != "hideFlags" && field.Name != "eventName")
            {
                object fieldValue = field.GetValue(soundContainer);
                if (fieldValue != null)
                {
                    soundObjectInfo.Add(field.Name, JToken.FromObject(fieldValue, serializer));
                }
            }
        }

        // Merge serialized fields into jObject
        infoToWrite.Merge(soundObjectInfo);

        // Write the final JSON object to the writer
        infoToWrite.WriteTo(writer);
    }
}