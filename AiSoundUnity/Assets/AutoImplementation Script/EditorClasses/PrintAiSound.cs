using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System.Linq;

public class PrintAiSound
{
    [MenuItem("Assets/AiSound/Print AiSound Object", validate = true)]
    private static bool ValidatePrintAiSoundToConsole()
    {
        // Only display the context menu item if an AiSound asset is selected
        foreach (Object obj in Selection.objects)
        {
            if (obj is not AiSound)
                return false;
        }
        return true;
    }

    [MenuItem("Assets/AiSound/Print AiSound Object")]
    private static void PrintAiSoundToConsole()
    {
        // Only display the context menu item if an AiSound asset is selected
        foreach (AiSound soundContainer in Selection.objects.Cast<AiSound>())
        {
            soundContainer.DebugDisplay();
        }
    }
}
