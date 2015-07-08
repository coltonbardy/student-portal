(function($){
	$(document).ready(function(){
		var base_path = '';
		var lfz = {};

		if(Drupal.settings && Drupal.settings.basePath){
			base_path = Drupal.settings.basePath;
		}
		if(Drupal.settings && Drupal.settings.lfz){
			lfz = Drupal.settings.lfz;
		}

		$('body').on('change', '#edit-select-class', function(elm){
			var target = elm.currentTarget;
			var class_nid = $(target).val();
			var url = "";
			if(lfz && lfz.redirect_url){
				url = base_path+lfz.redirect_url.replace("{class_nid}", class_nid);
			}else{
				console.error("Redirect url not defined");
				return;	
			}

			window.location = url;
		});
	});
})(jQuery);