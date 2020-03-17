<?php
/**
 * Provides a search for all chado stock-based Tripal Content regardless of type.
 *
 */
class LIMSrawseq extends ChadoCustomSearch {
  /**
   * The human readable title of this search. This will be used in listings
   * and shown on the search page as the title.
   */
  public static $title = 'Raw Sequence Search ';
  /**
   * The machine name of the module providing this search.
   */
  public static $module = 'lims';
  /**
   * A description of this search. This is shown at the top of the search page
   * and used for the menu item.
   */
  public static $description = '';
  /**
   * The machine names of the permissions with access to this search. This is
   * used to map your search to existing permissions. It must be an array and
   * is used in hook_menu(). Some examples include 'access content'
   * and 'administer tripal'.
   */
  public static $permissions = ['access lims'];
  /**
   * If TRUE, this search will require the submit button to be clicked before
   * executing the query; whereas, if FALSE it will be executed on the
   * first page load without parameters.
   *
   * NOTE: to control the results without parameters check $this->submitted
   * in getQuery() and if FALSE return your pre-submit query.
   */
  public static $require_submit = FALSE;
  /**
   * Add a pager to search results
   * with $num_items_per_page results on a single page.
   * NOTE: Your query has to handle paging.
   */
  public static $pager = TRUE;
  public static $num_items_per_page = 50;
  /**
   * This defined the hook_menu definition for this search. At a minimum the
   * path is required.
   */
  public static $menu = [
    'path' => 'search/sequences/raw',
    // @todo support menu items.
  ];
  /**
   * Add CSS/JS to the form/results page.
   * NOTE: paths supplied should be relative to $module.
   */
  public static $attached = [
    'css' => [
      'theme/lims-seqrun.search.css',
    ],
    'js' => [],
  ];
  /**
   * Information regarding the fields and filters for this search.
   */
  public static $info = [
    // Lists the columns in your results table.
    'fields' => [
      'run_name' => [
        'title' => 'Name',
      ],
      'library_type' => [
        'title' => 'Library',
      ],
      'technology' => [
        'title' => 'Technology',
      ],
      'germplasm' => [
        'title' => 'Germplasm Name',
      ],
      'species' => [
        'title' => 'Source Species',
      ],
    ],
    // The filter criteria available to the user.
    // This is used to generate a search form which can be altered.
    'filters' => [
      'genus' => [
        'title' => 'Genus',
        'help' => 'The scientific name of the germplasm the sequencing was performed on.',
      ],
      'species' => [
        'title' => 'Species',
        'help' => 'The species of the germplasm the sequencing was performed on.',
      ],
      'technology' => [
        'title' => 'Technology',
        'help' => 'The sequencing platform.'
      ],
      'read_type' => [
        'title' => 'Read Type',
        'help' => 'The method used to generate the reads.'
      ],
      'library_type' => [
        'title' => 'Library Type',
        'help' => 'The method of library preparation for sequencing.',
      ],
      'germplasm' => [
        'title' => 'Germplasm Name',
        'help' => 'The name of the germplasm you are interested in sequencing for.',
      ],
      'run_name' => [
        'title' => 'Name',
        'help' => 'The name of the sequencing run (partial names are accepted).',
      ],
    ],
  ];
  /**
   * Text that should appear on the button at the bottom of the importer
   * form.
   */
  public static $button_text = 'Search';
  /**
   * Generate the filter form.
   *
   * The base class will generate textfields for each filter defined in $info,
   * set defaults, labels and descriptions, as well as, create the search
   * button.
   *
   * Extend this method to alter the filter form.
   */
  public function form($form, $form_state) {
    $form = parent::form($form, $form_state);

    // Make Genus and Species Drop-downs.
    $description = LIMSrawseq::$info['filters']['genus']['help'];
    $form['genus']['#prefix'] = '<div id="scientific-nameform-item" class="scientific-name-element form-item"><label>Scientific Name</label>';
    $form['species']['#suffix'] = '<div class="description">' . $description . '</div></div>';
    unset($form['genus']['#description'], $form['species']['#description']);

    $genus = [];
    $species = [];
    $resource = chado_query('SELECT genus, species FROM {organism} WHERE organism_id IN (SELECT organism_id FROM [lims_seqrun_species])');
    foreach ($resource as $r) {
      $genus[$r->genus] = $r->genus;
      $species[$r->species] = $r->species;
    }

    $form['genus']['#type'] = 'select';
    $form['genus']['#options'] = $genus;
    $form['genus']['#empty_option'] = '- Select Genus -';
    $form['species']['#type'] = 'select';
    $form['species']['#options'] = $species;
    $form['species']['#empty_option'] = '- Select Species -';

    // Make Technology Drop-down.
    $tech =  db_query('SELECT technology FROM {lims_seqrun} GROUP BY technology')->fetchAllKeyed(0,0);
    $form['technology']['#type'] = 'select';
    $form['technology']['#options'] = $tech;
    $form['technology']['#empty_option'] = '- Select Technology -';

    // Make Read Type Drop-down.
    $read_type =  db_query('SELECT read_type FROM {lims_seqrun} GROUP BY read_type')->fetchAllKeyed(0,0);
    $form['read_type']['#type'] = 'select';
    $form['read_type']['#options'] = $read_type;
    $form['read_type']['#empty_option'] = '- Select Read Type -';

    // Make Library Type Drop-down.
    $library_type = db_query('SELECT library_type FROM {lims_seqrun} GROUP BY library_type')->fetchAllKeyed(0,0);
    $form['library_type']['#type'] = 'select';
    $form['library_type']['#options'] = $library_type;
    $form['library_type']['#empty_option'] = '- Select Library Type -';

    return $form;
  }

  /**
   * Allows custom searches to validate the form results.
   * Use form_set_error() to signal invalid values.
   */
  public function validateForm($form, $form_state) {
  }

  /**
   * Determine the query for the genetic marker search.
   *
   * @param string $query
   *   The full SQL query to execute. This will be executed using chado_query()
   *   so use curly brackets appropriately. Use :placeholders for any values.
   * @param array $args
   *   An array of arguments to pass to chado_query(). Keys must be the
   *   placeholders in the query and values should be what you want them set to.
   */
  public function getQuery(&$query, &$args, $offset) {
    global $user;

    // Retrieve the filter results already set.
    $filter_results = $this->values;

    // @debug dpm($filter_results, '$filter_results');
    $query = "SELECT
        main.run_name,
        main.technology,
        main.library_type,
        string_agg(o.genus||' '||o.species, ', ') as species,
        single_stock.name as germplasm
      FROM [lims_seqrun] main
      LEFT JOIN [lims_seqrun_species] sr_sp ON sr_sp.run_id=main.run_id
      LEFT JOIN {organism} o ON o.organism_id=sr_sp.organism_id
      LEFT JOIN [lims_seqrun_samples] single ON main.sample_id=single.sample_id
      LEFT JOIN {stock} single_stock ON single_stock.stock_id IS NULL OR single_stock.stock_id=single.sample_stock_id";

    $where = [];
    $joins = [];
    // -- Genus.
    if ($filter_results['genus'] != '') {
      $where[] = "o.genus ~* :genus";
      $args[':genus'] = $filter_results['genus'];
    }
    // -- Species.
    if ($filter_results['species'] != '') {
      $where[] = "main.run_id IN (SELECT subss.run_id FROM [lims_seqrun_species] subss
      LEFT JOIN {organism} subo ON subo.organism_id=subss.organism_id WHERE subo.species~*:species)";
      $args[':species'] = $filter_results['species'];
    }
    // -- Germplasm
    if ($filter_results['germplasm'] != '') {
      // Where [single sample name] = Redberry OR [sample provided by file] = Redberry.
      $where[] = "((single.sample_name~*:germ OR single_stock.name~*:germ)
        OR main.run_id IN (
          SELECT m.run_id
          FROM lims_seqrun_samples s
          INNER JOIN lims_seqrun m ON m.samples_fid=s.samples_fid
          LEFT JOIN chado.stock cs ON cs.stock_id=s.sample_stock_id
          WHERE s.sample_name~*:germ OR cs.name~*:germ))";
      $args[':germ'] = $filter_results['germplasm'];
    }
    // -- Technology.
    if ($filter_results['technology'] != '') {
      $where[] = "main.technology ~* :technology";
      $args[':technology'] = $filter_results['technology'];
    }
    // -- Read Type.
    if ($filter_results['read_type'] != '') {
      $where[] = "main.read_type ~* :read_type";
      $args[':read_type'] = $filter_results['read_type'];
    }
    // -- Library Type.
    if ($filter_results['library_type'] != '') {
      $where[] = "main.library_type ~* :library_type";
      $args[':library_type'] = $filter_results['library_type'];
    }
    // -- Name.
    if ($filter_results['run_name'] != '') {
      $where[] = "main.run_name ~* :run_name";
      $args[':run_name'] = $filter_results['run_name'];
    }

    // Finally, add it to the query.
    if (!empty($joins)) {
      $query .= implode("\n",$joins);
    }
    if (!empty($where)) {
      $query .= "\n" . ' WHERE ' . implode(' AND ',$where);
    }

    // Group it.
    $query .= "\n" . ' GROUP BY main.run_id, main.run_name, main.technology, main.library_type, single_stock.name';
    // Sort even though it is SLOW b/c ppl expect it.
    $query .= "\n" . ' ORDER BY main.run_id DESC';

    // Handle paging.
    $limit = $this::$num_items_per_page + 1;
    $query .= "\n" . ' LIMIT ' . $limit . ' OFFSET ' . $offset;
    // @debug dpm(strtr(str_replace(['{','}'], ['chado.', ''], $query), $args), 'query');
  }

  /**
   * Format the results within the $form array.
   *
   * The base class will format the results as a table.
   *
   * @param array $form
   *   The current form array.
   * @param array $results
   *   The results to format. This will be an array of standard objects where
   *   the keys map to the keys in $info['fields'].
   */
  public function formatResults(&$form, $results) {
    parent::formatResults($form, $results);

    foreach ($form['results']['#rows'] as $k => &$row) {

      // Make the run name a link.
      $nid = db_query('SELECT nid FROM {lims_seqrun} WHERE run_name=:name',
        [':name' => $row['run_name']])->fetchField();
      if ($nid) {
        $row['run_name'] = l($row['run_name'], 'node/'.$nid);
      }

      // Provide default for germplasm column.
      if (empty($row['germplasm'])) {
        $row['germplasm'] = '<span class="multi-germ">mutliple samples</span>';
      }
    }
  }
}
