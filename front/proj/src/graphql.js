
import gql from 'graphql-tag'

export const ROUTE_QUERY = gql`
{
    getRouteMap{
         _id
        direction
        time
  }
}
`

export const SUBSCRIPTION_ROUTE_MAP =gql`
subscription {
  routeAdd{
        _id
        direction
        time
  }
}
`