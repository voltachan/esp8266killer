var settingsJson = {};
function load() {
    getFile("settings.json",
    function(res) {
        settingsJson = JSON.parse(res);
        draw()
    })
}
function draw() {
    var html = "";
    for (var key in settingsJson) {
        key = esc(key);
        if (settingsJson.hasOwnProperty(key)) {
        if (typeof settingsJson[key] != "boolean"){
            html += "<div class='settings'>" + "" + "<label class='settingName " + (typeof settingsJson[key] == "boolean" ? "labelFix": "") + "' for='" + key + "'>" + key + ":</label>" + "" + "";
            }else{
            html += "<div class='settings'></div>" ;
            }}
            if (typeof settingsJson[key] == "boolean") {
                html += "<li><input type='checkbox' name='checkbox' class='labelauty' id='"+key+"' "+ " onchange='save(\"" + key + '",!settingsJson["' + key + "\"])'"+"style='display: none'"+ (settingsJson[key] ? " checked": "") +"><label for='"+key+"'><span class='labelauty-unchecked-image'></span><span class='labelauty-unchecked'>" + key + "</span><span class='labelauty-checked-image'></span><span class='labelauty-checked'>"+key+"</span></label></li>"+"</label></li>"
            } else {
                if (typeof settingsJson[key] == "number") {
                    html += "<input type='number' name='" + key + "' value=" + settingsJson[key] + " onchange='save(\"" + key + "\",parseInt(this.value))'>"
                } else {
                    if (typeof settingsJson[key] == "string") {
                        html += "<input type='text' name='" + key + "' value=" + settingsJson[key] + " " + (key == "version" ? "readonly": "") + " onchange='save(\"" + key + "\",this.value)'>"
                    }
                }
            }
        }
    
    getE("settingsList").innerHTML = html
}
function save(key, value) {
    if (key) {
        settingsJson[key] = value;
        getFile("run?cmd=set " + key + ' "' + value + '"')
    } else {
        getFile("run?cmd=save settings",
        function(res) {
            load()
        })
    }
};