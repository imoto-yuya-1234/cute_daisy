Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
		var info = Pebble.getActiveWatchInfo();
		console.log('Pebble model: ' + info.platform);
		
		var url;
		if (info.platform == "chalk") {
			url = "https://imoto-yuya-1234.github.io/cute_daisy/index_chalk.html";
		}
		else {
			url = "https://imoto-yuya-1234.github.io/cute_daisy/index_chalk.html";
		}
    Pebble.openURL(url);
		console.log("Showing configuration page: " + url);
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
		
		var configData = JSON.parse(decodeURIComponent(e.response));
  	console.log('Configuration page returned: ' + JSON.stringify(configData));

  	var dict = {};
		
  	var connection = configData['connection'];  	
    dict['KEY_CONNECTION'] = configData['connection'] ? 1 : 0;  // Send a boolean as an integer
		
		var show_ticks = configData['show_ticks'];
		dict['KEY_SHOW_TICKS'] = configData['show_ticks'] ? 1 : 0;
		
		var decoration = configData['decoration'];
		//dict['KEY_DECORATION'] = configData.decoration;
		dict['KEY_DECORATION'] = configData['decoration'];
  	
		var lang = configData['lang'];
		dict['KEY_LANG'] = configData['lang'];
		
		// Send to watchapp
		Pebble.sendAppMessage(dict, function() {
			console.log('Send successful: ' + JSON.stringify(dict));
		}, function() {
			console.log('Send failed!');
		});
  }
);