// Prints information about the selected objects to the console to help when developing scripts.

studio.menu.addMenuItem({
    name: 'What Is This?',

    execute: function()
    {
        var selDeck = studio.window.deckCurrent();
        var selBrowser = studio.window.browserCurrent();
        var selEditor = studio.window.editorCurrent();

        print("\n What Is This?")
        if (selEditor != null) {
            print("\n ~~~~~~~ Selected in Editor: " + selEditor + ", name: " + selEditor.name + " (type of " + selEditor.entity + "). \n ~~~~~~");
            selEditor.dump();
        }
        if (selBrowser != null) {
            print("\n ~~~~~~~ \n Selected in Browser: " + selBrowser + ", name: " + selBrowser.name + " (type of " + selBrowser.entity + "). \n ~~~~~~");
            selBrowser.dump();
        }
        if (selDeck != null) {
            print("\n ~~~~~~~ \n Selected in Deck: " + selDeck + ", name: " + selDeck.name + " (type of " + selDeck.entity + ")\n ~~~~~~");
            selDeck.dump();
        }
    }
});


function print(message)
{
    studio.system.print(message);
}