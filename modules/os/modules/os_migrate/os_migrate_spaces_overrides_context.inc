<?php 

class SpacesOverridesContextMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object = $this->dest_object = 'context';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    
    $query->join('og', 'og', 'og.nid = s.id');

    
    /* Only use contexts from the currently selected theme.  In d7 we aren't storing context overrides based on theme anymore, so no use migrating the rest.
     * This is the equivalent of 'WHERE  object_id LIKE CONCAT("%", og_theme) '
     * but dbtng requires the placeholder, even for a string like this. */
    $query->where('object_id LIKE CONCAT(:per, og_theme) ', array(':per' => '%'));
    
    return $query;
  }
  
  
  
  public function prepareRow(&$row) {
    $value = unserialize($row->value);
    $object_id = $row->object_id;
    

    list($object_id, $value) = $this->_update_context($object_id, $value, $row->id);
    if (!isset($value, $object_id)) {
      return FALSE; //skip this row.
    }
    
    $row->object_id = $object_id;
    $row->value = serialize($value);
    return parent::prepareRow($row);
  }
  

  /**
   * @function _update_context
   *
   * @param string $object_id
   *   identifier of context
   * @param array $value
   *   Array of blocks provided by context
   * @param int $nid
   *   Node id of the vsite providing this group
   *
   * @return boolean|array
   *   Returns updated $object_id and $value parameters or FALSE.
   */
  public function _update_context($object_id, array $value, $nid) {
  
    //Update the key to use the new context name.  Or bail out to drop the row.
    if (($object_id = $this->_update_context_key($object_id, $nid)) === FALSE) {
      return FALSE;
    }
     
    //boxes in values
    foreach ($value['blocks'] as $id => $block) {
      //update id
      $new_id = $id;
      unset($value['blocks'][$id]);
  
      //and block
      //(02:40:42 PM) Matt Petrovic: context only needs module, delta, region, weight, status and title
      $keep = array('module', 'delta', 'region', 'weight', 'status', 'title');
      $block = array_intersect_key($block, array_flip($keep));
      $block['title'] = '';
      $value['blocks'][$new_id] = $this->_update_block($block);
    }
  
    return array($object_id, $value);
  }
  
  //update a context's key.  return false if we're dropping it.
  public function _update_context_key($object_id, $nid) {
    //old to new mapping
    $contexts = array(
      'openscholar-section-sitewide' => FALSE, //no obvious mapping.  nothing in db
      'openscholar-section-sitewide_front' => 'os_sitewide_front',
      'vsite-section-cp' => 'os_cp',
      //'vsite-section-public' => 'os_public', //not used in overrides
      'openscholar-feature-image_gallery' => 'gallery_gallery',
      'openscholar-feature-front' => 'os_front',
      'openscholar-feature-none' => 'os_public', //all sections
    );

    list($context, $action, $type, $theme) = explode(':', $object_id);

    //update the context name
    if (isset($contexts[$context])) {
      $context = $contexts[$context];
    } else {
      $context = preg_replace('/openscholar-feature-(\w+)/', '$1_$1', $context); //err?
    }
  
    //$theme = $this->_update_theme_name($theme, FALSE);
    $object_id = implode(':', array($context, $action, $type));
    return $object_id;
  }
  
}
