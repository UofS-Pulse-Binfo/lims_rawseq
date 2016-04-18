<?php
/**
 * This template is used to render a teaser form of the sequencing run.
 */
$node = $node['#node'];
$seqrun = $node->seqrun;
?>
<div class="tripal_stock-teaser tripal-teaser">
  <div class="tripal-stock-teaser-text tripal-teaser-text teaser-items">
    <div class="item">
      <div class="item-title">Name</div>
      <div class="item-value"><?php print $seqrun->run_name; ?></div>
    </div>
    <div class="item">
      <div class="item-title">Library Type</div>
      <div class="item-value"><?php print $seqrun->library_type?></div>
    </div>
    <div class="item">
      <div class="item-title">Technology</div>
      <div class="item-value"><?php print $seqrun->technology; ?></div>
    </div>
    <div class="item">
      <div class="item-title">Species</div>
      <?php $species = array();
        foreach ($node->species as $s) {
          $species[] = '<i>' . $s->genus . ' ' . $s->species . '</i>';
        } ?>
      <div class="item-value"><?php print implode(', ', $species); ?></div>
    </div>

  </div>
</div>
