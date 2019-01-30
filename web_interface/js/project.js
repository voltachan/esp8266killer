function initPickerMap(){pickerMap=new google.maps.Map(document.getElementById("ui_picker_map_wrap"),{center:{lat:0,lng:0},disableDefaultUI:!0,mapTypeId:google.maps.MapTypeId.ROADMAP,zoom:15}),pickerMarker=new google.maps.Marker({map:pickerMap,position:{lat:0,lng:0}})}var $pickerLib=$(".ui-picker-lib"),pickerMap,pickerMarker;"undefined"!=typeof google&&initPickerMap(),"undefined"!=typeof jQuery.ui&&($(".ui-picker-draggable-handler").draggable({addClasses:!1,appendTo:"body",cursor:"move",cursorAt:{top:0,left:0},delay:100,helper:function(){return $('<div class="tile tile-brand-accent ui-draggable-helper"><div class="tile-side pull-left"><div class="avatar avatar-sm"><strong>'+$(".ui-picker-selected:first .ui-picker-draggable-avatar strong").html()+'</strong></div></div><div class="tile-inner text-overflow">'+$(".ui-picker-selected:first .ui-picker-info-title").html()+"</div></div>")},start:function(a,b){var c=$(".ui-picker-selected").length;c>1&&$(".ui-draggable-helper").append('<div class="avatar avatar-brand avatar-sm ui-picker-draggable-count">'+c+"</div>")},zIndex:100}),$(".ui-picker-nav .nav a").droppable({accept:".ui-picker-draggable-handler",addClasses:!1,drop:function(a,b){$("body").snackbar({content:'Dropped on "'+$(this).html()+'"'})},hoverClass:"ui-droppable-helper",tolerance:"pointer"}),$pickerLib.selectable({cancel:".ui-picker-draggable-handler",filter:".ui-picker-selectable-handler",selecting:function(a,b){var c=$(b.selecting).parent();c.addClass("tile-brand-accent ui-picker-selected"),$(".ui-picker-info").addClass("ui-picker-info-active").removeClass("ui-picker-info-null"),$(".ui-picker-info-desc-wrap").html(c.find(".ui-picker-info-desc").html()),$(".ui-picker-info-title-wrap").html(c.find(".ui-picker-info-title").html());var d=new google.maps.LatLng(c.find(".ui-picker-map-lat").html(),c.find(".ui-picker-map-lng").html());pickerMap.setCenter(d),pickerMarker.setMap(pickerMap),pickerMarker.setPosition(d)},unselecting:function(a,b){var c=$(b.unselecting).parent();if(c.removeClass("tile-brand-accent ui-picker-selected"),$(".ui-picker-selected").length){var d=$($(".ui-picker-selected")[0]);$(".ui-picker-info-desc-wrap").html(d.find(".ui-picker-info-desc").html()),$(".ui-picker-info-title-wrap").html(d.find(".ui-picker-info-title").html());var e=new google.maps.LatLng(d.find(".ui-picker-map-lat").html(),d.find(".ui-picker-map-lng").html());pickerMap.setCenter(e),pickerMarker.setMap(pickerMap),pickerMarker.setPosition(e)}else $(".ui-picker-info").addClass("ui-picker-info-null"),$(".ui-picker-info-desc-wrap").html(""),$(".ui-picker-info-title-wrap").html(""),pickerMarker.setMap(null)}})),$(document).on("click",".ui-picker-info-close",function(){$(".ui-picker-info").removeClass("ui-picker-info-active")}),$("#ui_datepicker_example_1").pickdate(),$("#ui_datepicker_example_2").pickdate({cancel:"Clear",closeOnCancel:!1,closeOnSelect:!0,container:"",firstDay:1,format:"You selecte!d: dddd, d mm, yy",formatSubmit:"dd/mmmm/yyyy",ok:"Close",onClose:function(){$("body").snackbar({content:"Datepicker closes"})},onOpen:function(){$("body").snackbar({content:"Datepicker opens"})},selectMonths:!0,selectYears:10,today:""}),$("#ui_datepicker_example_3").pickdate({disable:[[2016,0,12],[2016,0,13],[2016,0,14]],today:""}),$("#ui_datepicker_example_4").pickdate({disable:[new Date(2016,0,12),new Date(2016,0,13),new Date(2016,0,14)],today:""}),$("#ui_datepicker_example_5").pickdate({disable:[2,4,6],today:""}),$("#ui_datepicker_example_6").pickdate({disable:[{from:[2016,0,12],to:2}],today:""}),$("#ui_datepicker_example_7").pickdate({disable:[!0,3,[2016,0,13],new Date(2016,0,14)],today:""}),$("#ui_datepicker_example_8").pickdate({disable:[{from:[2016,0,10],to:[2016,0,30]},[2016,0,13,"inverted"],{from:[2016,0,19],to:[2016,0,21],inverted:!0}],today:""}),$("#ui_datepicker_example_9").pickdate({max:[2016,0,30],min:[2016,0,10],today:""}),$("#ui_datepicker_example_10").pickdate({max:new Date(2016,0,30),min:new Date(2016,0,10),today:""}),$("#ui_datepicker_example_11").pickdate({max:!0,min:-10,today:""}),$(".finish-loading").on("click",function(a){a.stopPropagation(),$($(this).attr("data-target")).addClass("el-loading-done")}),$("#ui_el_loading_example_wrap .tile-active-show").each(function(a){var b,c=$(this);c.on("hide.bs.tile",function(a){clearTimeout(b)}),c.on("show.bs.tile",function(a){$(".el-loading",c).hasClass("el-loading-done")||(b=setTimeout(function(){$(".el-loading",c).addClass("el-loading-done"),c.prepend('<div class="tile-sub"><p>Additional information<br><small>Aliquam in pharetra leo. In congue, massa sed elementum dictum, justo quam efficitur risus, in posuere mi orci ultrices diam.</small></p></div>')},6e3))})});var snackbarText=1;$("#ui_snackbar_toggle_1").on("click",function(){$("body").snackbar({content:"Simple snackbar "+snackbarText+" with some text",show:function(){snackbarText++}})}),$("#ui_snackbar_toggle_2").on("click",function(){$("body").snackbar({content:'<a data-dismiss="snackbar">Dismiss</a><div class="snackbar-text">Simple snackbar '+snackbarText+' with some text and a simple <a href="javascript:void(0)">link</a>.</div>',show:function(){snackbarText++}})});