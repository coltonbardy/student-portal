<?php
/**
 * Created by PhpStorm.
 * User: ericjohnson
 * Date: 7/21/15
 * Time: 5:06 PM
 */
$media_alignment = (isset($media_alignment))?$media_alignment:"media-left";
$media_html = (isset($media_html))?$media_html:"";
?>

<div class="media">
    <div class="<?php print $media_alignment;?>">
        <?php print $media_html; ?>
    </div>
    <div class="media-body">
        <h3 class="media-heading"><?php print $heading; ?></h3>
        <?php print $content; ?>
    </div>
</div>