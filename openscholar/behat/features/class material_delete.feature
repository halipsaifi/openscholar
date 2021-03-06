Feature: Testing the deletion for a class material.

  @api
  Scenario: Verify when a class is deleted, the class material will also deleted.
    Given I am logged in as a user with the "administrator" role
     When I create a new "class" entry with the name "Dummy class entry"
      And I click "Add class material"
      And I fill in "Title" with "Dummy class material"
      And I press "edit-submit"
     Then I should verify the node "Dummy class material" not exists
