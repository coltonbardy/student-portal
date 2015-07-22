<?php
$day_of_week = date('l', $time);
$month = date('F', $time);
$day = date('d', $time);
$date = date("Y-d-m", $time);
drupal_add_css("time.icon
    {
        font-size: .4em; /* change icon size */
        display: block;
        position: relative;
        width: 7em;
        height: 7em;
        background-color: #fff;
        margin: 2em auto;
        border-radius: 0.5em;
        box-shadow: 0 1px 0 #bdbdbd, 0 2px 0 #fff, 0 3px 0 #bdbdbd, 0 4px 0 #fff, 0 5px 0 #bdbdbd, 0 0 0 1px #bdbdbd;
        overflow: hidden;
        -webkit-backface-visibility: hidden;
        -webkit-transform: rotate(0deg) skewY(0deg);
        -webkit-transform-origin: 50% 10%;
        transform-origin: 50% 10%;
    }

    time.icon *
    {
        display: block;
        width: 100%;
        font-size: 1em;
        font-weight: bold;
        font-style: normal;
        text-align: center;
    }

    time.icon strong
    {
        position: absolute;
        top: 0;
        padding: 0.28em 0;
        color: #fff;
        background-color: #fd9f1b;
        border-bottom: 1px dashed #f37302;
        box-shadow: 0 2px 0 #fd9f1b;
    }

    time.icon em
    {
        position: absolute;
        bottom: 0.3em;
        color: #fd9f1b;
    }

    time.icon span
    {
        width: 100%;
        font-size: 2.8em;
        letter-spacing: -0.05em;
        padding-top: 0.8em;
        color: #2f2f2f;
    }

    time.icon:hover, time.icon:focus
    {
        -webkit-animation: swing 0.6s ease-out;
        animation: swing 0.6s ease-out;
    }", "inline")
?>
<div class="media-left">
    <time datetime="<?php print $date; ?>" class="icon">
        <em><?php print $day_of_week; ?></em>
        <strong><?php print $month; ?></strong>
        <span><?php print $day; ?></span>
    </time>
</div>