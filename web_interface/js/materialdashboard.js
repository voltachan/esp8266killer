(function(){isWindows=navigator.platform.indexOf("Win")>-1?true:false;if(isWindows&&!$("body").hasClass("sidebar-mini")){$(".sidebar .sidebar-wrapper, .main-panel").perfectScrollbar();$("html").addClass("perfect-scrollbar-on")}else{$("html").addClass("perfect-scrollbar-off")}})();var breakCards=true;var searchVisible=0;var transparent=true;var transparentDemo=true;var fixedTop=false;var mobile_menu_visible=0,mobile_menu_initialized=false,toggle_initialized=false,bootstrap_nav_initialized=false;var seq=0,delays=80,durations=500;var seq2=0,delays2=80,durations2=500;$(document).ready(function(){$sidebar=$(".sidebar");$.material.init();md.initSidebarsCheck();if($("body").hasClass("sidebar-mini")){md.misc.sidebar_mini_active=true}window_width=$(window).width();md.initMinimizeSidebar();if($(".selectpicker").length!=0){$(".selectpicker").selectpicker()}if(breakCards==true){$('[data-header-animation="true"]').each(function(){var $fix_button=$(this);var $card=$(this).parent(".card");$card.find(".fix-broken-card").click(function(){console.log(this);var $header=$(this).parent().parent().siblings(".card-header, .card-image");$header.removeClass("hinge").addClass("fadeInDown");$card.attr("data-count",0);setTimeout(function(){$header.removeClass("fadeInDown animate")},480)});$card.mouseenter(function(){var $this=$(this);hover_count=parseInt($this.attr("data-count"),10)+1||0;$this.attr("data-count",hover_count);if(hover_count>=20){$(this).children(".card-header, .card-image").addClass("hinge animated")}})})}$('input[type="checkbox"][required="true"], input[type="radio"][required="true"]').on("click",function(){if($(this).hasClass("error")){$(this).closest("div").removeClass("has-error")}})});$(document).on("click",".navbar-toggle",function(){$toggle=$(this);if(mobile_menu_visible==1){$("html").removeClass("nav-open");$(".close-layer").remove();setTimeout(function(){$toggle.removeClass("toggled")},400);mobile_menu_visible=0}else{setTimeout(function(){$toggle.addClass("toggled")},430);var $layer=$('<div class="close-layer"></div>');if($("body").find(".main-panel").length!=0){$layer.appendTo(".main-panel")}else{if(($("body").hasClass("off-canvas-sidebar"))){$layer.appendTo(".wrapper-full-page")}}setTimeout(function(){$layer.addClass("visible")},100);$layer.click(function(){$("html").removeClass("nav-open");mobile_menu_visible=0;$layer.removeClass("visible");setTimeout(function(){$layer.remove();$toggle.removeClass("toggled")},400)});$("html").addClass("nav-open");mobile_menu_visible=1}});$(window).resize(function(){md.initSidebarsCheck()});md={misc:{navbar_menu_visible:0,active_collapse:true,disabled_collapse_init:0,},checkSidebarImage:function(){$sidebar=$(".sidebar");image_src=$sidebar.data("image");if(image_src!==undefined){sidebar_container='<div class="sidebar-background" style="background-image: url('+image_src+') "/>';$sidebar.append(sidebar_container)}},initSliders:function(){var slider=document.getElementById("sliderRegular");noUiSlider.create(slider,{start:40,connect:[true,false],range:{min:0,max:100}});var slider2=document.getElementById("sliderDouble");noUiSlider.create(slider2,{start:[20,60],connect:true,range:{min:0,max:100}})},initSidebarsCheck:function(){if($(window).width()<=991){if($sidebar.length!=0){md.initRightMenu()}}},initMinimizeSidebar:function(){$("#minimizeSidebar").click(function(){var $btn=$(this);if(md.misc.sidebar_mini_active==true){$("body").removeClass("sidebar-mini");md.misc.sidebar_mini_active=false}else{$("body").addClass("sidebar-mini");md.misc.sidebar_mini_active=true}var simulateWindowResize=setInterval(function(){window.dispatchEvent(new Event("resize"))},180);setTimeout(function(){clearInterval(simulateWindowResize)},1000)})},checkScrollForTransparentNavbar:debounce(function(){if($(document).scrollTop()>260){if(transparent){transparent=false;$(".navbar-color-on-scroll").removeClass("navbar-transparent")}}else{if(!transparent){transparent=true;$(".navbar-color-on-scroll").addClass("navbar-transparent")}}},17),initRightMenu:debounce(function(){$sidebar_wrapper=$(".sidebar-wrapper");if(!mobile_menu_initialized){$navbar=$("nav").find(".navbar-collapse").children(".navbar-nav.navbar-right");mobile_menu_content="";nav_content=$navbar.html();nav_content='<ul class="nav nav-mobile-menu">'+nav_content+"</ul>";navbar_form=$("nav").find(".navbar-form").get(0).outerHTML;$sidebar_nav=$sidebar_wrapper.find(" > .nav");$nav_content=$(nav_content);$navbar_form=$(navbar_form);$nav_content.insertBefore($sidebar_nav);$navbar_form.insertBefore($nav_content);$(".sidebar-wrapper .dropdown .dropdown-menu > li > a").click(function(event){event.stopPropagation()});window.dispatchEvent(new Event("resize"));mobile_menu_initialized=true}else{if($(window).width()>991){$sidebar_wrapper.find(".navbar-form").remove();$sidebar_wrapper.find(".nav-mobile-menu").remove();mobile_menu_initialized=false}}},200),};function debounce(func,wait,immediate){var timeout;return function(){var context=this,args=arguments;clearTimeout(timeout);timeout=setTimeout(function(){timeout=null;if(!immediate){func.apply(context,args)}},wait);if(immediate&&!timeout){func.apply(context,args)}}};