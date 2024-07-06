(require '[clj-http.client :as client])
(require '[cheshire.core :refer :all])
(print (parse-string ((client/get "http://127.0.0.1:8123/tuningserver") :body)))
