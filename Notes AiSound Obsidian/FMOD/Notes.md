https://alessandrofama.com/tutorials/fmod/fmod-studio/scripting-api-basics
- Can use strings for labeled parameters : `setParameterByNameWithLabel` in unity for example
- If you want to create a menu item, you need to use `studio.menu.addMenuItem`
	- To be able to launch it from the editor don't include the `()` to your function in `execute`
	- If you do, it will launch it on `reload` and be greyed out
- For some reason, Fmod hates `let` and requires instead `var`
- Use `dump` and the console to try to understand wtf is going on
	- When you define an object in the console you can go object. and all it's properties will show up

