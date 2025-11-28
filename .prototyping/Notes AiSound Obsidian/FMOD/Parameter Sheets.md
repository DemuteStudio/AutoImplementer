- For some reason, creating parameter sheets doesn't visually refresh them, I need to figure out how to trigger a visual Fmod refresh
	- Actually it was just cuz the parameter's maximum was set to a number smaller than the amount of labels
```js
const paramArr = ["label1", "label2","label3","label4","label5","label6"];  
param.preset.enumerationLabels = paramArr;  
param.preset.maximum = paramArr.length;
```
