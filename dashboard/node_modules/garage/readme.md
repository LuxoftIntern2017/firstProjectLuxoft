# Garage

Is a simple data file storgage. It stores files according to a key and can retrieve them with the same key again.

## Install

````
npm install garage
````

## Usage

```` javascript
var garage = require("garage");
var storeage = new garage("./path/to/your/storage");

storeage.put('key','data', function(err){
	// ... file stored
});

storeage.get('key', function(err, data){
	// ... file retrieved
});

````
