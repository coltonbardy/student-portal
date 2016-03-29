<?php
/**
 * Created by PhpStorm.
 * User: ericjohnson
 * Date: 3/10/16
 * Time: 3:56 PM
 */ ?>

Website Form Auto Responder,

The form (<?php print $form; ?>) form was submitted on the website with the following information:

<?php foreach ($form_values as $key => $value):
    if(!is_string($value)){
        continue;
    }
    ?>

    <p><b><?php print $key; ?></b> : <?php print $value; ?></p>

<?php endforeach; ?>

You can take more action by going into the <?php print $link; ?>
