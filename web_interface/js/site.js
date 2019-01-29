var langJson = {};

function getE(name){
	return document.getElementById(name);
}

function esc(str) {
	if(str){
		return str.toString()
			.replace(/&/g, '&amp;')
			.replace(/</g, '&lt;')
			.replace(/>/g, '&gt;')
			.replace(/\"/g, '&quot;')
			.replace(/\'/g, '&#39;')
			.replace(/\//g, '&#x2F;');
	}
	return "";
}

function convertLineBreaks(str){
	if(str){
		str = str.toString();
		str = str.replace(/(?:\r\n|\r|\n)/g,'<br>');
		return str;
	}
	return "";
}

function showMessage(msg, closeAfter){
	var elmt = getE("error");
	elmt.innerHTML = esc(msg)+"<a onclick='closeMessage()' id='closeError'>x</a>";
	
	elmt.classList.remove('hide');
	elmt.classList.add('show');

	if(closeAfter !== undefined){
		setTimeout(closeMessage(),closeAfter);
	}
}

function closeMessage(){
	var elmt = getE("error");
	elmt.innerHTML = "";
	elmt.classList.remove('show');
	elmt.classList.add('hide');
}

function getFile(adr, callback, timeout, method, onTimeout, onError){
	/* fallback stuff */
	if(adr === undefined) return;
	if(callback === undefined) callback = function(){};
	if(timeout === undefined) timeout = 8000; 
	if(method === undefined) method = "GET";
	if(onTimeout === undefined) {
		onTimeout = function(){
			showMessage("错误：文件加载超时！文件名："+adr);
		};
	}
	if(onError === undefined){
		onError = function(){
			showMessage("错误：无法加载文件！文件名："+adr);
		};
	}
	
	/* create request */
	var request = new XMLHttpRequest();
	
	/* set parameter for request */
	request.open(method, encodeURI(adr), true);
	request.timeout = timeout;
	request.ontimeout = onTimeout;
    request.onerror = onError;
	request.overrideMimeType("application/json");
	
	request.onreadystatechange = function() {
		if(this.readyState == 4){
			if(this.status == 200){
				callback(this.responseText);
			}
		}
	};
	
	/* send request */
	request.send();
	
	console.log(adr);
}