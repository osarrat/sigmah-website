// $Id: webfm_image.js,v 1.1 2009/02/08 06:24:05 robmilne Exp $
/**
 * @file webfm_image.js
 *
 * Handles AJAX submission and response in WebFM Browser UI.
 */
function webfm_image() {}

if (Drupal.jsEnabled) {
  $(window).load(webfm_imageGetMenusAjax);
}

webfm_image.ajaxUrl = function() {
  var path = getBaseUrl() + "/?q=webfm_image_js";
  return path;
}

function webfm_imageGetMenusAjax() {
  var url = webfm_image.ajaxUrl();
  Webfm.progressObj.show(Webfm.js_msg["work"],  "blue");
  var postObj = {action:encodeURIComponent("get_menus")};
  Webfm.HTTPPost(url, webfm_image.GetMenusCallback, '', postObj);
}

webfm_image.GetMenusCallback = function(string, xmlhttp, obj) {
  Webfm.progressObj.hide();
  Webfm.alrtObj.msg();
  if (xmlhttp.status == 200) {
    var result = Drupal.parseJson(string);
//    Webfm.dbgObj.dbg("GetMenusCallback:", Webfm.dump(result));
    if (result.status) {
      var menus = [];
      menus = result.data;
      for(var i = 0; i < menus.length; i++) {
        Webfm.menuHT.put('file', new Webfm.menuElement("Resize to "+menus[i], webfm_image.menuResize, webfm_image.menuTest));
      }
    } else {
      Webfm.alrtObj.msg(result.data);
    }
  } else {
    Webfm.alrtObj.msg(Webfm.js_msg["ajax-err"]);
  }
}

webfm_image.menuResize = function(obj){
  var url = webfm_image.ajaxUrl();
  var path = obj.element.title;
  Webfm.progressObj.show(Webfm.js_msg["work"],  "blue");
  var postObj = {action:encodeURIComponent(this.desc), filepath:encodeURIComponent(path)};
  Webfm.dbgObj.dbg("postObj:", Webfm.dump(postObj));
  Webfm.HTTPPost(url, webfm_image.ResizeCallback, obj, postObj);
}

webfm_image.ResizeCallback = function(string, xmlhttp, obj) {
  Webfm.progressObj.hide();
  Webfm.alrtObj.msg();
  if (xmlhttp.status == 200) {
    var result = Drupal.parseJson(string);
//    Webfm.dbgObj.dbg("resize result:", Webfm.dump(result));
    if (result.status) {
      Webfm.dirListObj.refresh();
//      Webfm.alrtObj.str_arr(result.data);
    } else {
      Webfm.alrtObj.msg(result.data);
    }
  } else {
    Webfm.alrtObj.msg(Webfm.js_msg["ajax-err"]);
  }
}

webfm_image.menuTest = function(obj) {
  switch(obj.ext.toLowerCase()) {
    case 'jpg':
    case 'jpeg':
    case 'png':
    case 'bmp':
    case 'gif':
      return true;
    default:
      return false;
  }
}
