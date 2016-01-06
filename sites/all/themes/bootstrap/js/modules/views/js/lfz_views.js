(function ($) {

    $(document).ready(function(){
        $('body').on('click', ".assign-queue-to-me", function(){
            var url = 'question-queue/ajax/assign';

            var dataObj = {
                'uid':$(this).data('uid')
            };

            $(this).attr("disabled", "disabled");


            $.ajax({
                url: url,
                data: dataObj,
                type:'POST',
                dataType:'json',
                success:function(response){
                    if(response['success']){
                        $(this).attr("disabled", "");
                    }else{
                        $(this).addClass("btn-error");
                    }
                }
            });
        });

        $('body').on('click', ".unassign-queue-to-me", function(){
            var url = 'question-queue/ajax/unassign';

            var dataObj = {
                'uid':$(this).data('uid')
            };

            $(this).attr("disabled", "disabled");


            $.ajax({
                url: url,
                data: dataObj,
                type:'POST',
                dataType:'json',
                success:function(response){
                    if(response['success']){
                        $(this).attr("disabled", "");
                    }else{
                        $(this).addClass("btn-error");
                    }
                }
            });
        });
    });


})(jQuery);
