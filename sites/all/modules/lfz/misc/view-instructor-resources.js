/**
 * Created by ericjohnson on 7/22/15.
 */

//this is included inside the boostrap theme in a view template file
jQuery(document).ready(function () {

    var base_path = '';
    if(Drupal.settings && Drupal.settings.basePath){
        base_path = Drupal.settings.basePath;
    }

    jQuery('body').on('click', '.agenda-add-today', function () {
        console.log("click called");
        var item_nid = (jQuery(this).data('resource-nid'))?jQuery(this).data('resource-nid'):false;

        if(item_nid){

            var url = base_path+'ajax/today/add-item';

            var obj = {
                'item_nid': item_nid,
            };

            jQuery.ajax({
                url:url,
                data:obj,
                dataType:'json',
                success:function(response){
                    console.log("response : ", response);
                    if(response.success){
                    }else{
                    }
                },
                error:function(response){

                }
            });
        }else{
            //error getting nid
            console.error("couldn't get nid from element from : ", jQuery(this));
        }
    });
});